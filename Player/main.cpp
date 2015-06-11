#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include "myPipe.hpp"

using namespace std;

HANDLE hWritePipe, hReadPipe;

char tmpStr[1024];

const double TimeLimit = 1.0;
const int board_size = 15;
int turn,row,column;
int board[board_size*board_size]; // 0 for black 1 for white 2 for empty
int temp_board[board_size*board_size];
int dx[8] = {0,0,1,-1,1,-1,1,-1},dy[8] = {1,-1,1,-1,0,0,-1,1};
int state[2][2][5]; //[black/white][unblock/block][count]

bool check_range(int x,int y) {
    return (x >= 0 && x < board_size && y >= 0 && y < board_size);
}

pair <int, bool> c_line(int point, int turn, int direction, int length) {
    pair <int,bool> result;
    int x, y;
    y = point / board_size + dx[direction];
    x = point % board_size + dy[direction];
    result.second = false;
    result.first = length;
    if(check_range(x, y)) {
        if(board[y * board_size+x] == board[point]) {
            result = c_line(y * board_size + x, turn, direction, length+1);
        } else if(board[y * board_size+x]!= 2) {
            result.second = true;
        }
    }
    return result;
}
int check_win() {
    for(int i = 0; i<board_size*board_size; ++i) {
        if(board[i] == 2) {
            continue;
        }
        for(int k = 0; k < 8; k += 2) {
            pair <int,bool> left,right;
            left = c_line(i, board[i], k,0);
            right = c_line(i, board[i], k + 1, 0);
            if(left.first + right.first + 1 >= 5) {
                return board[i];
            }
        }
    }
    return 2;
}
void create_state() {
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 1; k <= 4; ++k) {
                state[i][j][k] = 0;
            }
        }
    }
    for(int i = 0; i < board_size * board_size; ++i) {
        if(board[i] == 2) {
            continue;
        }
        for(int k = 0; k < 8; k += 2) {
            pair <int,bool> left,right;
            left = c_line(i, board[i], k, 0);
            right = c_line(i, board[i], k+1, 0);
            if(left.second == true && right.second == true) {
                continue;
            }
            if(left.second == false && right.second == false) {
                state[board[i]][0][min(left.first+right.first+1,4)]++;
            } else {
                state[board[i]][1][min(left.first+right.first+1,4)]++;
            }
        }
    }
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 1; k <= 4; ++k) {
                state[i][j][k] /= k;
            }
        }
    }
}
vector <int> search_point() {
    vector <int> point;
    for(int i = 0; i<board_size; ++i) {
        for(int j = 0; j<board_size; ++j) {
            if(board[i*board_size+j] == 0 || board[i*board_size+j] == 1) {
                for(int k = 0; k<8; ++k) {
                    int y = i+dy[k],x = j+dx[k];
                    if(check_range(x,y) && board[y*board_size+x] == 2 && find(point.begin(),point.end(),y*board_size+x) == point.end()) { //empty place
                        point.push_back(y*board_size+x);
                    }
                }
            }
        }
    }
    return point;
}
int get_score() {
    int (*selfState)[5] = state[turn];
    int (*opponentState)[5] = state[!turn];

    return selfState[0][1] + selfState[0][2] * 2 + selfState[0][3] * 10 + selfState[0][4] * 100
    - opponentState[0][1] + opponentState[0][2] * 2 + opponentState[0][3] * 10 + opponentState[0][4] * 100;
}
void show() {
    system("cls");
    for(int i = 0; i<board_size; ++i) {
        for(int j = 0; j<board_size; ++j) {
            if(board[i*board_size+j] == 0) {
                cout<<"○";
            } else if(board[i*board_size+j] == 1) {
                cout<<"●";
            } else {
                cout<<"‧";
            }
        }
        cout<<endl;
    }
}
int alpha_beta_puring(int alpha,int beta,int turn,int deep,int limit) {
    int check;
    check = check_win();
    if(check != 2) {
        return check == turn ? INT_MIN : INT_MAX;
    }
    if(deep == limit+1) {
        create_state();
        return get_score();// 從盤面狀態回傳分數
    }
    vector <int> use_point = search_point();
    for(int i = 0; i < use_point.size(); ++i) {
        board[use_point[i]] = turn;
        if(deep%2 == 0) { //Max level
            alpha = max(alpha,alpha_beta_puring(alpha,beta,(turn+1)%2,deep+1,limit));
        } else { //Min level
            beta = min(beta,alpha_beta_puring(alpha,beta,(turn+1)%2,deep+1,limit));
        }
        board[use_point[i]] = 2;
        if(alpha>= beta) {
            break;
        }
    }
    if(deep%2 == 0) {
        return alpha;
    } else {
        return beta;
    }
}

void sendPut(int row, int col) {
    sprintf(tmpStr, "%d %d", row, col);
    sendMsg(hWritePipe, tmpStr);

    printf("put on [%02d,%02d].\n", row, col);
}

void getPut(int &row, int &col) {
    getMsg(tmpStr, hReadPipe); //get opponent move
    sscanf(tmpStr, "%d%d", &row, &col);

    printf("Opponent put on [%02d,%02d].\n", row, col);
}

int main(int argc, char* argv[]) {
//initial game
    int empty_place;
    srand(time(0));

    for(int i = 0; i < board_size * board_size; ++i) {
        board[i] = 2;
    }
    empty_place = board_size * board_size;

//something about pipe start
    cout << argv[1] << endl;

    string writePipeName("\\\\.\\pipe\\From");
    string readPipeName("\\\\.\\pipe\\To");

    if(argc < 2) {
        cout << "Error args!!" << endl;
        system("pause");
        return 1;
    }

    writePipeName.append(argv[1]);
    readPipeName.append(argv[1]);

    connectPipe(hWritePipe, writePipeName.c_str());

    if (hWritePipe == INVALID_HANDLE_VALUE) {
        cout << "Can't find pipe: From" << argv[1] << endl;
        system("pause");
        return 1;
    }

    createPipe(hReadPipe, readPipeName.c_str()); //create pipe
    if (hReadPipe == INVALID_HANDLE_VALUE) {
        cout << "Can't create pipe"<< endl;
        system("pause");
        return 1;
    }

    sendMsg(hWritePipe, "r"); //tell GUI the pipe is created
    cout << "send" << endl;
    ConnectNamedPipe(hReadPipe, NULL); //wait for GUI connect to pipe

    cout << "is connect" << endl;
    getMsg(tmpStr, hReadPipe); //get info -> black or white

    cout << "ready" << endl;
//something about pipe end

    if(tmpStr[0] == 'b') {
        cout << "I am black." << endl;
        board[1 * board_size + 7] = 0;
        turn = 1; //white
        empty_place --;
        sendPut(1, 7);
    } else {
        cout << "I am white." << endl;
    }

    show();


    while(check_win() == 2 && empty_place > 0) { //win_check
        //wait for user input
        int row, col;
        getPut(row, col);
        board[row * board_size + col] = turn;
        turn = (turn+1) % 2;
        show();

        if(check_win() != 2 || empty_place <= 0) break;

        int deep = 3, point;
        vector <int> use_point;
        use_point = search_point();
        point = use_point[0];
        for(int i = 0; i<board_size*board_size; ++i) {
            temp_board[i] = board[i];
        }
        time_t start_tm, finish_tm;
        time(&start_tm);
        time(&finish_tm);
        while(difftime(finish_tm,start_tm) < TimeLimit) { //time_check
            int Max_score,score;
            Max_score = INT_MIN;
            for(int i = 0; i<use_point.size(); ++i) {
                board[use_point[i]] = turn;
                score = alpha_beta_puring(INT_MIN,INT_MAX,turn,1,deep);
                if(score == INT_MIN) {
                    use_point.erase(use_point.begin()+i);
                    i--;
                    continue;
                }
                if(score > Max_score) {
                    Max_score = score;
                    point = use_point[i];
                }
                board[use_point[i]] = 2; //empty
            }
            deep++;
            if(Max_score == INT_MAX) { //必贏
                break;
            }
            time(&finish_tm);
        }

        //robot put
        temp_board[point] = turn;
        turn = (turn+1) % 2;
        empty_place--;

        for(int i = 0; i < board_size * board_size; ++i) {
            board[i] = temp_board[i];
        }

        sendPut(point/board_size, point%board_size);


        show();

        create_state();

        cout << "0 0 X :";
        for(int i = 0; i < 5; i++){
           cout << state[0][0][i] << endl;
        }

        cout << "0 1 X :";
        for(int i = 0; i < 5; i++){
           cout << state[0][1][i] << endl;
        }
    }
    system("pause");
    return 0;
}


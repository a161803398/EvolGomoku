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
#include "GpTree.h"

using namespace std;

HANDLE hWritePipe, hReadPipe;

char tmpStr[1024];

const double TimeLimit = 3.0;
const int board_size = 15;
int turn,row,column;
int board[board_size*board_size]; // 0 for black 1 for white 2 for empty
int temp_board[board_size*board_size];
int dx[8] = {0, 0, 1, -1, 1, -1, 1, -1},dy[8] = {1, -1, 1, -1, 0, 0, -1, 1};
int state[2][2][5]; //[black/white][unblock/block][count]
int (*selfState)[5];
int (*oppState)[5];
GeneTree *evalFunTree;

int evalTree(GeneTree *p) {
    if(!p) return 0;

    int opd1 = evalTree(p->left);
    int opd2 = evalTree(p->right);

    switch(p->opt) {
    case '+':
        return opd1 + opd2;
    case '-':
        return opd1 - opd2;
    case '*':
        return opd1 * opd2;
    case '/':
        if(opd2 == 0) return opd1;
        return opd1 / opd2;
    case '%':
        if(opd2 == 0) return opd1;
        return opd1 % opd2;
    case AZ:
        return opd1 > 0 ? opd2 : 0;
    case AEZ:
        return opd1 >= 0 ? opd2 : 0;
    case EZ:
        return opd1 == 0 ? opd2 : 0;
    case NZ:
        return opd1 != 0 ? opd2 : 0;
    case EL:
        return opd1 != 0 ? opd1 : opd2;
    case 'n':
        return p->data;
    case SL:
        return selfState[0][p->data];
    case SBL:
        return selfState[1][p->data];
    case OL:
        return oppState[0][p->data];
    case OBL:
        return oppState[1][p->data];
    default: //that should not happen
        cout << "Error, unknown opt !!!" << std::endl;
    }
}

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
                for(int k = 0; k < 8; ++k) {
                    int y = i + dy[k], x = j + dx[k];
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
    selfState = state[!turn];
    oppState = state[turn];
    int totalScore = evalTree(evalFunTree);
    return turn ? totalScore : -totalScore;
}


void show() {
    //system("cls");
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


int alphabeta(int depth, int alpha, int beta, bool maximizingPlayer) {
    int check = check_win();

    if(check != 2) {
        return (check == 0 ? (INT_MAX - 100 + depth) : (INT_MIN + 100 - depth));
    }

    if (depth == 0) {
        create_state();
        return get_score();// 從盤面狀態回傳分數
    }

    if (maximizingPlayer) {
        int v = INT_MIN;
        vector <int> use_point = search_point();
        for(int i = 0; i < use_point.size(); i++) {
            board[use_point[i]] = turn; //put
            turn = !turn;

            v = max(v, alphabeta(depth - 1, alpha, beta, false));
            alpha = max(alpha, v);

            board[use_point[i]] = 2; //undo
            turn = !turn;
            if(alpha >= beta) break;
        }
        return v;
    } else {
        int v = INT_MAX;
        vector <int> use_point = search_point();
        for(int i = 0; i < use_point.size(); i++) {
            board[use_point[i]] = turn; //put
            turn = !turn;

            v = min(v, alphabeta(depth - 1, alpha, beta, true));
            beta = min(beta, v);

            board[use_point[i]] = 2; //undo
            turn = !turn;
            if(alpha >= beta) break;
        }
        return v;
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
        system("StartGui.exe");
        return 1;
    }


    ifstream fin("gene.txt");
    importTree(evalFunTree, fin);
    fin.close();


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

        int deep = 1, point;
        vector <int> use_point;
        use_point = search_point();

        point = use_point[0];
        for(int i = 0; i < board_size*board_size; ++i) {
            temp_board[i] = board[i];
        }
        time_t start_tm, finish_tm;
        time(&start_tm);
        time(&finish_tm);

        bool isBlack = !turn;

        while(difftime(finish_tm,start_tm) < TimeLimit) { //time_check
            int best_score, score;
            vector <pair<int, int> > use_point_score;
            use_point_score.reserve(use_point.size());

            if(isBlack) {
                best_score = INT_MIN;

                for(int i = 0; i < use_point.size(); ++i) {
                    board[use_point[i]] = turn;
                    turn = !turn;

                    score = alphabeta(deep, best_score, INT_MAX, false);
                    cout << "put on [" << use_point[i]/board_size << "," <<  use_point[i]%board_size << "] : " << score << endl;
                    /*
                                        if(score == INT_MIN) {
                                            use_point.erase(use_point.begin()+i);
                                            i--;
                                            continue; //may cause bug(no board[use_point[i]] = 2;)
                                        }
                    */
                    if(score != INT_MIN) { //if not lose
                        use_point_score.push_back(std::make_pair(score, use_point[i]));
                    }

                    if(score > best_score) {
                        best_score = score;
                        point = use_point[i];
                    }
                    board[use_point[i]] = 2; //empty
                    turn = !turn;
                }

                //sort point base on score
                std::sort(use_point_score.begin(), use_point_score.end(), std::greater< pair<int, int> >());
                use_point.clear();
                for(int i = 0; i < use_point_score.size(); i++) use_point.push_back(use_point_score[i].second);
                if(best_score > (INT_MAX - 100)) { //必贏
                    break;
                }
            } else {
                best_score = INT_MAX;
                for(int i = 0; i < use_point.size(); ++i) {
                    board[use_point[i]] = turn;
                    turn = !turn;

                    score = alphabeta(deep, INT_MIN, best_score, true);
                    cout << "put on [" << use_point[i]/board_size << "," <<  use_point[i]%board_size << "] : " << score << endl;

                    if(score != INT_MIN) { //if not lose
                        use_point_score.push_back(std::make_pair(score, use_point[i]));
                    }

                    if(score < best_score) {
                        best_score = score;
                        point = use_point[i];
                    }
                    board[use_point[i]] = 2; //empty
                    turn = !turn;
                }

                //sort point base on score
                std::sort(use_point_score.begin(), use_point_score.end());
                use_point.clear();
                for(int i = 0; i < use_point_score.size(); i++) use_point.push_back(use_point_score[i].second);
                if(best_score < (INT_MIN + 100)) { //必贏
                    break;
                }
            }
            deep++;
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
        cout << "0, 0, X" << endl << endl;
        for(int i =0; i < 5; i++) cout << state[0][0][i] << endl;

        cout << "0, 1, X" << endl << endl;
        for(int i =0; i < 5; i++) cout << state[0][1][i] << endl;

        cout << "search deep : " << deep << endl;
    }
    system("pause");
    return 0;
}


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
#include <thread>
#include "myPipe.hpp"
#include "GpTree.h"

#define showInfo
using namespace std;

HANDLE hWritePipe, hReadPipe;

char tmpStr[1024];

const int SCORE_MAX = INT_MAX / 2;
const int SCORE_MIN = INT_MIN / 2;

const double TimeLimit = 10.0;
const int board_size = 15;
int turn,row,column;
int board[board_size*board_size]; // 0 for black 1 for white 2 for empty
int temp_board[board_size*board_size];
int dx[8] = {0, 0, 1, -1, 1, -1, 1, -1},dy[8] = {1, -1, 1, -1, 0, 0, -1, 1};
int state[2][2][5]; //[black/white][unblock/block][count]
int (*selfState)[5];
int (*oppState)[5];
GeneTree *evalFunTree;

int nextPoint;
int depthLimit = 1;
bool forceStop = false;
bool isThinking = false;

vector <int> use_point;
int empty_place = board_size * board_size;

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
        cout << "Error, unknown opt :" << p->opt << std::endl;
        return 0;
    }
}

bool check_range(int x,int y) {
    return (x >= 0 && x < board_size && y >= 0 && y < board_size);
}

void putOn(int idx) {
    board[idx] = turn;
    turn = (turn+1) % 2;
    empty_place--;
}

void putOn(int row, int col) {
    putOn(row * board_size + col);
}

void undoPut(int idx) {
    empty_place++;
    turn = (turn+1) % 2;
    board[idx] = 2;
}

void undoPut(int row, int col) {
    undoPut(row * board_size + col);
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
    /*
    for(int i = 0; i<board_size; ++i) {
        for(int j = 0; j<board_size; ++j) {
            if(board[i * board_size + j] == 2) point.push_back(i * board_size + j);
        }
    }*/

    return point;
}
/*
int get_score() {
    selfState = state[!turn];
    oppState = state[turn];
    int totalScore = evalTree(evalFunTree);
    return turn ? totalScore : -totalScore;
}
*/

int get_score() {
    int (*selfState)[5] = state[!turn];
    int (*oppState)[5] = state[turn];

    int totalScore = 0;


    if(selfState[0][3] >= 2) totalScore += 800; //雙活三
    if(selfState[0][3] > 0 && selfState[1][4] > 0) totalScore += 800;//活三 四

    totalScore +=  selfState[0][3] * 100 + //活三得 200分
                   selfState[1][4] * 10 + //四得20分
                   selfState[1][3] * 5 + //三得10分
                   selfState[0][2] * 2 +//活二得4分
                   selfState[0][1] / 2;//活一得1分

    if(oppState[0][4] > 0) totalScore -= 900; //活四
    if(oppState[1][4] >= 2) totalScore -= 900; //雙四
    if(oppState[0][3] >= 2) totalScore -= 800; //雙活三
    if(oppState[0][3] > 0 && oppState[1][4] > 0) totalScore -= 800;//活三 四

    totalScore -=  oppState[0][3] * 100 + //活三得 200分
                   oppState[1][4] * 10 + //四得20分
                   oppState[1][3] * 5 + //三得10分
                   oppState[0][2] * 2 +//活二得4分
                   oppState[0][1] / 2;//活一得1分

    if(oppState[1][4] > 0 || oppState[0][4] > 0) totalScore -= 9000;
    if(oppState[0][3] > 0)oppState -= 700;

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
    if(forceStop) return 0; //force stop
    int check = check_win();

    if(check != 2) {
        return (check == 0 ? (SCORE_MAX - depth) : (SCORE_MIN + depth));
    }

    if(empty_place <= 0) return 0; //even

    if (depth >= depthLimit) {
        create_state();
        return get_score();// 從盤面狀態回傳分數
    }

    if (maximizingPlayer) {
        int v = SCORE_MIN;
        vector <int> use_point = search_point();
        for(int i = 0; i < use_point.size(); i++) {
            putOn(use_point[i]);

            v = max(v, alphabeta(depth + 1, alpha, beta, false));
            alpha = max(alpha, v);

            undoPut(use_point[i]);

            if(forceStop) return 0; //force stop
            if(alpha >= beta) break;
        }
        return v;
    } else {
        int v = SCORE_MAX;
        vector <int> use_point = search_point();
        for(int i = 0; i < use_point.size(); i++) {
            putOn(use_point[i]);

            v = min(v, alphabeta(depth + 1, alpha, beta, true));
            beta = min(beta, v);

            undoPut(use_point[i]);

            if(forceStop) return 0; //force stop
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


void thinking() {
    bool isBlack = !turn;
    depthLimit = 1;

    while(!forceStop) {
        int best_score, score;
        vector <pair<int, int> > use_point_score;
        use_point_score.reserve(use_point.size());

        vector <int > extraSearchList;
        use_point_score.reserve(8);


        best_score = SCORE_MIN;

        int i = 0, j = 0;
        bool normalSearch = true;
        while(normalSearch || j < extraSearchList.size()) {
            int curSearch;
            if(i < use_point.size()) {
                curSearch = use_point[i];
                i++;
            } else if(extraSearchList.empty()) { //create extraSearchList
                for(int k = 0; k < 8; ++k) {

                    int x = nextPoint % board_size + dx[k];
                    int y = nextPoint / board_size + dy[k];

                    if(check_range(x,y) && board[y * board_size + x] == 2 && find(use_point.begin(), use_point.end(), y * board_size + x) == use_point.end()) { //empty place
                        extraSearchList.push_back(y * board_size + x);
                    }
                }
                normalSearch = false;
#ifdef showInfo
                cout << "extra size : " << extraSearchList.size() << endl;
#endif // showInfo

                continue; //avoid redundant search
            } else {
                curSearch = extraSearchList[j];
                j++;
            }

            putOn(curSearch);

            score = isBlack ? alphabeta(0, SCORE_MIN, SCORE_MAX, false) : -alphabeta(0, SCORE_MIN, SCORE_MAX, true);

            undoPut(curSearch);

            if(forceStop) return; //force stop

#ifdef showInfo
            cout << "put on [" << curSearch / board_size << "," <<  curSearch % board_size << "] : " << score << endl;
#endif // showInfo
            if(score > SCORE_MIN + 100) { //if not lose
                use_point_score.push_back(std::make_pair(score, curSearch));
            }

            if(score > best_score) {
                best_score = score;
                nextPoint = curSearch;
            }

        }

        //sort point base on score
        std::sort(use_point_score.begin(), use_point_score.end(), std::greater< pair<int, int> >());
        use_point.clear();

        for(int i = 0; i < use_point_score.size(); i++) use_point.push_back(use_point_score[i].second);
        if(best_score > (SCORE_MAX - 100) || use_point_score.size() <= 1) { //必贏 || 只有一步能下
            break;
        }

        depthLimit ++; //add depth and start a new run

        cout << "now Search depth " << depthLimit << endl;
    }
    isThinking = false;
}


int main(int argc, char* argv[]) {
//initial game
    srand(time(0));

    for(int i = 0; i < board_size * board_size; ++i) {
        board[i] = 2;
    }

//something about pipe start
    cout << argv[1] << endl;

    string writePipeName("\\\\.\\pipe\\From");
    string readPipeName("\\\\.\\pipe\\To");

    if(argc < 3) {
        cout << "Error args!!" << endl;
        system("StartGui.exe");
        return 1;
    }


    ifstream fin(argv[2]);
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
#ifdef showInfo
    show();
#endif // showInfo



    while(check_win() == 2 && empty_place > 0) { //win_check
        //wait for user input
        int row, col;
        getPut(row, col);
        putOn(row, col);

#ifdef showInfo
        show();
#endif // showInfo

        if(check_win() != 2 || empty_place <= 0) break; //win_check

        use_point = search_point();
        nextPoint = use_point[0];

        time_t start_tm, finish_tm;
        time(&start_tm);
        time(&finish_tm);

        isThinking = true; //must put here
        thread thinkingThread(thinking);

        while(isThinking && difftime(finish_tm,start_tm) < TimeLimit) { //time_check
            time(&finish_tm);
        }
        forceStop = true;

#ifdef showInfo
        if(isThinking) {
            cout << "timeout try to end thinking thread!!" << endl;
        }

#endif // showInfo

        thinkingThread.join(); //wait thinking thread end
        isThinking = false;
        forceStop = false;

        //robot put
        putOn(nextPoint);
        sendPut(nextPoint/board_size, nextPoint%board_size);

#ifdef showInfo
        show();
#endif // showInfo
        /*
                create_state();
                cout << "0, 0, X" << endl << endl;
                for(int i =0; i < 5; i++) cout << state[0][0][i] << endl;

                cout << "0, 1, X" << endl << endl;
                for(int i =0; i < 5; i++) cout << state[0][1][i] << endl;
        */

    }

#ifdef showInfo
    system("pause");
#endif // showInfo

    return 0;
}


#include "board.h"
using namespace std;
int board[BOARD_COL * BOARD_ROW]; //range in 0, 1, 2, 3
int putCount = 0;
int curPlayer = BLACK;

int getStoneId(int row, int col){
    return (row + OFFSET) * BOARD_COL + (col + OFFSET);
}

void putOn(int row, int col) {
    putOn(curPlayer, row, col);
    curPlayer = (curPlayer == BLACK) ? WHITE : BLACK;
}

void putOn(int type, int row, int col) {
    int putId = getStoneId(row, col);
    if(board[putId] != NONE) {
        cout << "Error : illegal put" << endl;
        system("pause");
    } else {
        board[putId] = type;
        putCount ++;
    }
}

int getPartLink(int putId, int searchDirect) {
    int judgeType = board[putId];

    if(judgeType == NONE || judgeType == WALL) {
        return 0;
    }

    int searchId = putId;
    int linkCount = 0;

    while(true) {
        searchId += searchDirect;
        int curType = board[searchId];

        if(curType == judgeType) {
            linkCount++;
        } else {
            return linkCount;
        }
    }
}

int winCheck(int row, int col) {
    int place = (row + OFFSET) * BOARD_COL + (col + OFFSET);

    for(int i = 0; i < 4; i++) {
        int searchDirect1 = searchGroup[i];
        int searchDirect2 = -searchDirect1;

        int totalLink = 1 + getPartLink(place, searchDirect1) + getPartLink(place, searchDirect2);

        if(totalLink >= WIN_TRIGGER) {
            return board[place];
        }
    }

    if(putCount >= MAX_STONES) {
        return WALL;
    } else {
        return NONE;
    }
}

void printBoard() {
    for(int i = OFFSET; i < BOARD_COL - OFFSET; i++) {
        for(int j = OFFSET; j < BOARD_ROW - OFFSET; j++) {
            if(board[BOARD_COL * i + j]) {
                cout << (board[BOARD_COL * i + j] == 1 ? "¡³" : "¡´");
            } else {
                cout << "¡D";
            }
        }
        cout << endl;
    }
}

void resetBoard() {
    for(int i = 0; i < BOARD_COL * BOARD_ROW; i++) board[i] = 0;

    //fill the wall
    for(int i = 0; i < BOARD_COL; i++) {
        board[i] = 3; //fill first row
        board[BOARD_COL * (BOARD_ROW - 1) + i] = 3; //fill last row
    }
    for(int i = 1; i < BOARD_ROW - 1; i++) {
        board[BOARD_COL * i] = 3; //fill first column
        board[BOARD_COL * i + BOARD_COL - 1] = 3; //fill last column
    }
    putCount = 0;
}

int getPutCount() {
    return putCount;
}

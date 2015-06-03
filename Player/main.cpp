#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "myPipe.hpp"
#include "board.h"

using namespace std;

HANDLE hWritePipe, hReadPipe;

//#define debug
char tmpStr[1024];


void sendPut(int row, int col) {
#ifndef debug
    sprintf(tmpStr, "%d %d", row, col);
    sendMsg(hWritePipe, tmpStr);
#endif // debug
    putOn(row, col);
    printf("put on [%02d,%02d].\n", row, col);
}

void getPut(int &row, int &col) {
#ifdef debug
    printf("Please input:");
    scanf("%d%d", &row, &col);
#else
    getMsg(tmpStr, hReadPipe); //get opponent move
    cout << "already get" << tmpStr << endl;
    sscanf(tmpStr, "%d%d", &row, &col);
#endif // debug
    printf("Opponent put on [%02d,%02d].\n", row, col);
}

int main(int argc, char* argv[]) {
    resetBoard();
    srand(time(0));

    cout << argv[1] << endl;
#ifdef debug
    printf("Please input start:");
    scanf("%s", tmpStr);
#else
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
#endif // debug
    cout << "ready" << endl;

    if(tmpStr[0] == 'b') {
        cout << "I am black." << endl;
        sendPut(1, 7);
    } else {
        cout << "I am white." << endl;
    }
    int winner = NONE;

    //start loop
    while(winner == NONE) {
        int row, col;
        getPut(row, col);

        putOn(row, col);
        winner = winCheck(row, col);
        if(winner != NONE) break;

        while(board[getStoneId(row, col)] != NONE) {
            row = rand() % 15;
            col = rand() % 15;
        }
        sendPut(row, col);
        winner = winCheck(row, col);
    }

#ifndef debug
    CloseHandle(hWritePipe);
    DisconnectNamedPipe(hReadPipe);
    CloseHandle(hReadPipe);
#endif // debug
    cout << (winner == WALL ? "Even" : (winner == BLACK ? "Black Win!" : "White Win!")) << endl;
    //system("pause");
    return 0;
}



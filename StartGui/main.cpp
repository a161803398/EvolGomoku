#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "myPipe.hpp"
#include "board.h"

using namespace std;
bool callPlayer(HANDLE &fromPlayer, HANDLE &toPlayer, const char* playerName, const char* genePath);

char judgeName[1024] = "test";

#define showInfo
int main(int argc, char* argv[]) {

    HANDLE fromPlayer1, toPlayer1;
    HANDLE fromPlayer2, toPlayer2;
    char tmp[1024] = "";


    int opt = 0;
    printf("Please input gene file path =>");
    scanf("%s", tmp);

    printf("1) human first\n"
           "2) agent first\n");
    scanf("%d", &opt);


    if(!callPlayer(fromPlayer1, toPlayer1, opt == 1 ? "gui" : "Player", tmp)) {
        cout << "Player 1 do not respond, exit!!" << endl;
        return 1;
    }
    //cout << "callPlayer1 finish" << endl;
    if(!callPlayer(fromPlayer2, toPlayer2, opt == 1 ? "Player" : "gui", tmp)) {
        cout << "Player 2 do not respond, exit!!" << endl;
        return 1;
    }
    //cout << "callPlayer2 finish" << endl;

    int row, col;
    resetBoard();

    sendMsg(toPlayer1, "b"); //tell player1 that it is black
    //cout << "send b" << endl;

    sendMsg(toPlayer2, "w"); //tell player2 that it is white
    //cout << "send w" << endl;

    int winner = NONE;
    while(winner == NONE) {
        getMsg(tmp, fromPlayer1); //wait player1 respond
        sscanf(tmp, "%d%d", &row, &col); //resolve and print puts
        putOn(BLACK, row, col);
        sendMsg(toPlayer2, tmp); //tell player2 where player1 puts on

#ifdef showInfo
        system("cls");
        printBoard();
#endif // showInfo

        winner = winCheck(row, col);
        if(winner != NONE) break;
        //system("pause");

        getMsg(tmp, fromPlayer2); //wait player2 respond
        sscanf(tmp, "%d%d", &row, &col); //resolve and print puts
        putOn(WHITE, row, col);
        sendMsg(toPlayer1, tmp); //tell player1 where player2 puts on

#ifdef showInfo
        system("cls");
        printBoard();
#endif // showInfo


        winner = winCheck(row, col);
        //system("pause");
    }

    CloseHandle(toPlayer1); //write finish
    CloseHandle(toPlayer2); //write finish

    DisconnectNamedPipe(fromPlayer1); //read finish
    CloseHandle(fromPlayer1);
    DisconnectNamedPipe(fromPlayer2); //read finish
    CloseHandle(fromPlayer2);

    int player1Score = 0, player2Score = 0;
    if(winner != WALL) { //not even
        if(winner == BLACK) { //Player1 win
            player1Score = 255 - getPutCount();
            player2Score = -player1Score;
        } else { //Player2 win
            player2Score = 255 - getPutCount();
            player1Score = -player2Score;
        }
    }

#ifdef showInfo
    cout << "Game over" << endl;
    cout << "Player 1 get score :" << player1Score <<endl;
    cout << "Player 2 get score :" << player2Score <<endl;
#endif // showInfo

    system("pause");
    return 0;
}


bool callPlayer(HANDLE &fromPlayer, HANDLE &toPlayer, const char* playerName, const char* genePath) {
    char tmp[1024];
    char fullName[1024];

    sprintf(fullName, "%s_%s", playerName, judgeName);

    sprintf(tmp, "\\\\.\\pipe\\From%s", fullName);
    createPipe(fromPlayer, tmp); //create pipe for player1

    if (fromPlayer == INVALID_HANDLE_VALUE) {
        cout << "Can't create pipe"<< endl;
        system("pause");
        return false;
    }
    sprintf(tmp, "start %s.exe %s %s", playerName, fullName, genePath);
    system(tmp); //execute program

    cout << "waiting" << endl;

    ConnectNamedPipe(fromPlayer, NULL); //wait player connect

    getMsg(tmp, fromPlayer);

    if(tmp[0] != 'r') return false;
    cout << "get response" << endl;

    //listen to player pipe
    sprintf(tmp, "\\\\.\\pipe\\To%s", fullName);
    connectPipe(toPlayer, tmp);

    if (toPlayer == INVALID_HANDLE_VALUE) {
        cout << "Can't find pipe"<< endl;
        system("pause");
        return false;
    }
    return true;
}

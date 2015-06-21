#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "myPipe.hpp"

using namespace std;

int callJudge(int judgeId, const char* genePath1, const char* genePath2);

int main(int argc, char* argv[]) {
    char player1[1024], player2[1024];

    printf("Please input Player1 : ");
    scanf("%s", &player1);

    printf("Please input Player2 : ");
    scanf("%s", &player2);


    int player1Score = callJudge(0, player1, player2);

    cout << player1 <<" V.S. " << player2 << endl;
    cout << player1 <<" : " << player1Score << endl;
    cout << player2 <<" : " << -player1Score << endl;

    player1Score = callJudge(0, player2, player1);
    cout << player2 <<" V.S. " << player1 << endl;
    cout << player1 <<" : " << -player1Score << endl;
    cout << player2 <<" : " << player1Score << endl;


    system("pause");
    return 0;
}


int callJudge(int judgeId, const char* genePath1, const char* genePath2) {
    HANDLE fromJudge;
    char tmp[1024];

    sprintf(tmp, "\\\\.\\pipe\\FromJudge%d", judgeId);
    createPipe(fromJudge, tmp); //create pipe for judge
    sprintf(tmp, "start Judge.exe Judge%d %s %s", judgeId, genePath1, genePath2);

    system(tmp); //execute program

    ConnectNamedPipe(fromJudge, NULL);//wait juge connect

    getMsg(tmp, fromJudge);

    if(tmp[0] != 'r') return -1;

    getMsg(tmp, fromJudge);
    int player1Score;
    sscanf(tmp, "%d", &player1Score);

    DisconnectNamedPipe(fromJudge); //read finish
    CloseHandle(fromJudge);

    return player1Score;
}

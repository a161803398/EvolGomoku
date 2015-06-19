#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "myPipe.hpp"
#include "board.h"

using namespace std;

bool callJudge(HANDLE &fromJudge, int judgeId, const char* genePath1, const char* genePath2);
const int Population = 10;
int main(int argc, char* argv[]) {
    char tmp[1024];

    int score[Population] = {0};

    HANDLE fromJudges[Population];
    for(int j =0; j < 100; j++) {

        //start judges
        for(int i = 0; i < Population; i++) {
            if(!callJudge(fromJudges[i], i, "a", "b")) {
                cout << "Judge" <<  i << " do not respond, exit!!" << endl;
                return 1;
            }
        }

        //get scores from judges
        for(int i = 0; i < Population; i++) {
            getMsg(tmp, fromJudges[i]);
            int player1Score;
            sscanf(tmp, "%d", &player1Score);
            cout << "Judge " << i << endl;
            cout << "Player 1 get score :" << player1Score <<endl;
            cout << "Player 2 get score :" << -player1Score <<endl;
            DisconnectNamedPipe(fromJudges[i]); //read finish
            CloseHandle(fromJudges[i]);
        }
    }


    system("pause");
    return 0;
}


bool callJudge(HANDLE &fromJudge, int judgeId, const char* genePath1, const char* genePath2) {
    char tmp[1024];

    sprintf(tmp, "\\\\.\\pipe\\FromJudge%d", judgeId);
    createPipe(fromJudge, tmp); //create pipe for judge
    sprintf(tmp, "start Judge.exe Judge%d %s %s", judgeId, genePath1, genePath2);

    system(tmp); //execute program

    ConnectNamedPipe(fromJudge, NULL);//wait juge connect

    getMsg(tmp, fromJudge);

    if(tmp[0] != 'r') return false;
    return true;
}

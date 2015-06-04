#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "game.h"
#include "myPipe.hpp"
using namespace std;

HANDLE hWritePipe, hReadPipe;

char tmpStr[1024];


void sendPut(int step) {
    int row, col;
    transLoc(step, row, col);

    sprintf(tmpStr, "%d %d", row, col);
    sendMsg(hWritePipe, tmpStr);
    printf("put on [%02d,%02d].\n", row, col);
}

int getPut() {
    int row, col;
    getMsg(tmpStr, hReadPipe); //get opponent move
    cout << "already get" << tmpStr << endl;
    sscanf(tmpStr, "%d%d", &row, &col);
    printf("Opponent put on [%02d,%02d].\n", row, col);
    return transLoc(row, col);
}


int main (int argc, char* argv[]) {

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


    //redirects stdout, stderr
#ifdef dbm
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
#endif // dbm

    SDL_Event event;

    SDL_Surface *screen = NULL;
    SDL_Surface *board = NULL;
    SDL_Surface *background = NULL;
    SDL_Surface *stones = NULL;

    SDL_Surface *computerBtnImg = NULL;

    srand(time(0));

    if(!createWindow(&screen)) return 1;


    //load images
    board = loadImg("res\\borad.png");
    background = loadImg("res\\background.png");
    stones  = loadImg("res\\stones.png");
    computerBtnImg  = loadImg("res\\computer.png");


    //set stone
    Stone::setImage(stones, screen);
    Stone::setEvent(&event);

    //create game
    setGame();

    printf("Start Game\n");

    bool quit = false;

    //print background start
    applyImg(0,0,background ,screen);
    applyImg(600,0,background ,screen);
    applyImg(0,600,background ,screen);
    applyImg(600,600,background ,screen);
    applyImg(100,100,board ,screen);
    //print background end
    SDL_Flip(screen);


    if(tmpStr[0] == 'w') { //I am white
        realPut(getPut());
        drawStones();
        //Update the screen
        if(SDL_Flip(screen) == -1) quit = true; //quit on update fail

    }

    while(!quit) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) quit = true; //if X then quit
        }

        //Fill the screen white
        //SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        int playerPutId;

        if(playerPut(playerPutId)) { //update screen
            sendPut(playerPutId); //send put

            //Update the screen
            drawStones();
            if(SDL_Flip(screen) == -1) quit = true; //quit on update fail
            if(gameOverChk())continue;

            realPut(getPut()); //get put
            //Update the screen
            drawStones();
            if(SDL_Flip(screen) == -1) quit = true; //quit on update fail
            if(gameOverChk())continue;
        }
    }


    //free everything
    SDL_Quit();
    FreeConsole();

    return 0;
}

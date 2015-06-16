#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <set>
#include <map>
#include <vector>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include "Stone.h"
#include <time.h>

const int TRY_TIMIES = 1000;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;


const int WALL_SIZE = 1;
const int BOARD_COL = 15 + WALL_SIZE * 2;
const int BOARD_ROW = 15 + WALL_SIZE * 2;
const int TOTAL_BLOCK = BOARD_ROW * BOARD_COL;
const int MAX_STONES = (BOARD_ROW - WALL_SIZE * 2) * (BOARD_COL - WALL_SIZE * 2);
const int OFFSET = 122;

const int WIN_TRIGGER = 5;
const int MAX_SCORE = 1000000;


const int searchGroup[4] = {1, BOARD_COL, BOARD_COL + 1, BOARD_COL - 1}; //{-, |, \, /}
const int searchSpace[16] = {1, -1, //down, up
                            BOARD_COL, -BOARD_COL, //right, left
                            BOARD_COL + 1, -BOARD_COL - 1, //rightDown, leftUp
                            BOARD_COL - 1, -BOARD_COL + 1  //rightUp, leftDown
                           };


bool createWindow(SDL_Surface **screen);


void transLoc(int step, int &row, int &col);
int transLoc(int &row, int &col);
bool playerPut(int &putId);
void drawStones();

void updateNextStepList(int putId);

void realPut(int putId);

void showLinkedCount();
void showTable();

int getPartLink(int putId, int searchDirect);


STONE_TYPE getWinner();

void setGame();

bool gameOverChk();
void printLoc(int step);


static const char *black[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "          XXXXXXXXXXXX          ",
    "        XXXXXXXXXXXXXXXX        ",
    "       XXXXXXXXXXXXXXXXXX       ",
    "     XXXXXXXXXXXXXXXXXXXXXX     ",
    "    XXXXXXXXXXXXXXXXXXXXXXXX    ",
    "   XXXXXXXXXXXXXXXXXXXXXXXXXX   ",
    "   XXXXXXXXXXXXXXXXXXXXXXXXXX   ",
    "  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",
    " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
    " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
    " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX ",
    "  XXXXXXXXXXXXXXXXXXXXXXXXXXXX  ",
    "   XXXXXXXXXXXXXXXXXXXXXXXXXX   ",
    "   XXXXXXXXXXXXXXXXXXXXXXXXXX   ",
    "    XXXXXXXXXXXXXXXXXXXXXXXX    ",
    "     XXXXXXXXXXXXXXXXXXXXXX     ",
    "       XXXXXXXXXXXXXXXXXX       ",
    "        XXXXXXXXXXXXXXXX        ",
    "          XXXXXXXXXXXX          ",
    "0,0"
};

static const char *white[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "          ............          ",
    "        ................        ",
    "       ..................       ",
    "     ......................     ",
    "    ........................    ",
    "   ..........................   ",
    "   ..........................   ",
    "  ............................  ",
    " .............................. ",
    " .............................. ",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    " .............................. ",
    " .............................. ",
    "  ............................  ",
    "   ..........................   ",
    "   ..........................   ",
    "    ........................    ",
    "     ......................     ",
    "       ..................       ",
    "        ................        ",
    "          ............          ",
    "0,0"
};


static SDL_Cursor *init_system_cursor(const char *image[]);

//STONE_TYPE winCheck();

#endif // GAME_H_INCLUDED

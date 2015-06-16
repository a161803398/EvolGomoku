#include "game.h"
using std::set;
using std::vector;
using std::map;

//bool stoneWin[2];
int prePut;


vector<Stone> allStones(TOTAL_BLOCK);
set<int> nextStepList;


STONE_TYPE prePlay;
STONE_TYPE curPlay;
int stoneCount;

SDL_Cursor* blackCursor;
SDL_Cursor* whiteCursor;
SDL_Cursor* oriCursor;


void setGame() {
    stoneCount = 0;
    prePut = 0;
    prePlay = WHITE;
    curPlay = BLACK;

    blackCursor = init_system_cursor(black);
    whiteCursor = init_system_cursor(white);
    oriCursor = SDL_GetCursor();
    SDL_SetCursor(blackCursor);

    //memset(noBlockLinkCount, 0, sizeof(noBlockCount)); //0 only
    //memset(blockLinkCount, 0, sizeof(blockCount)); //0 only


    //stoneWin[0] = false;
    //stoneWin[1] = false;


    //place all stones
    for(int i = WALL_SIZE; i < BOARD_ROW - WALL_SIZE; i++) {
        for(int j = WALL_SIZE; j < BOARD_COL - WALL_SIZE; j++) {
            allStones[BOARD_COL * i + j].setPosition(OFFSET + STONE_WIDTH * (j - WALL_SIZE), OFFSET + STONE_HEIGHT * (i - WALL_SIZE));
        }
    }

    //fill the wall
    for(int i = 0; i < BOARD_COL; i++) {
        allStones[i].setType(WALL);//fill first row
        allStones[BOARD_COL * (BOARD_ROW - 1) + i].setType(WALL);//fill last row
    }
    for(int i = 1; i < BOARD_ROW - 1; i++) {
        allStones[BOARD_COL * i].setType(WALL);//fill first column
        allStones[BOARD_COL * i + BOARD_COL - 1].setType(WALL);//fill last column
    }

}


static SDL_Cursor *init_system_cursor(const char *image[]) {
    int i, row, col;
    Uint8 data[4*32];
    Uint8 mask[4*32];

    i = -1;
    for (row=0; row<32; ++row) {
        for (col=0; col<32; ++col) {
            if (col % 8) {
                data[i] <<= 1;
                mask[i] <<= 1;
            } else {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4+row][col]) {
            case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
            case '.':
                mask[i] |= 0x01;
                break;
            case ' ':
                break;
            }
        }
    }

    return SDL_CreateCursor(data, mask, 32, 32, 16, 16);
}

bool createWindow(SDL_Surface **screen) {
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
        return false;
    }

    *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if(*screen == NULL) {
        return false;
    }

    if(TTF_Init() == -1) {
        return false;
    }

    SDL_WM_SetCaption("Final Five", NULL);
    return true;
}

bool playerPut(int &putId) {
    for(int i = 0; i < TOTAL_BLOCK; i++) {
        //if((stoneCount > 0 || abs(i / BOARD_COL - 8) > 5 || abs(i % BOARD_COL - 8) > 5)) {
        if(allStones[i].isClick(curPlay)) {
            realPut(i);
            putId = i;
            return true;
        }
        //}
    }
    return false;
}

void drawStones() {
    for(int i = 0; i < TOTAL_BLOCK; i++) {
        allStones[i].display();
    }
}


bool gameOverChk() {
    switch(getWinner()) {
    case BLACK:
        Stone::setPause(true);
        SDL_SetCursor(oriCursor);
        printf("Black win!!");
        MessageBox(NULL, "Black win!!", "Gomoku", MB_ICONASTERISK);
        return true;
    case WHITE:
        Stone::setPause(true);
        SDL_SetCursor(oriCursor);
        printf("White win!!");
        MessageBox(NULL, "White win!!", "Gomoku", MB_ICONASTERISK);
        return true;
    case WALL:
        Stone::setPause(true);
        SDL_SetCursor(oriCursor);
        printf("Tie!!");
        MessageBox(NULL, "Tie!!", "Gomoku", MB_ICONASTERISK);
        return true;
    default:
        return false;
    }
}


void realPut(int putId) {
    printf("%d) %s on ", stoneCount, (curPlay == BLACK) ? "Black" : "White");
    printLoc(putId);

    allStones[prePut].setMarked(false);

    allStones[putId].setMarked(true);
    allStones[putId].setType(curPlay);


    stoneCount++;

    allStones[prePlay].display();
    allStones[putId].display();

    prePlay = curPlay;
    prePut = putId;
    curPlay = reverseStone(curPlay); //reverse
    SDL_SetCursor( (curPlay == BLACK) ? blackCursor : whiteCursor);
}


STONE_TYPE getWinner() {
    for(int i = 0; i < 4; i++) {
        int searchDirect1 = searchGroup[i];
        int searchDirect2 = -searchDirect1;


        int totalLink = 1 + getPartLink(prePut, searchDirect1) + getPartLink(prePut, searchDirect2);

        if(totalLink >= WIN_TRIGGER) {
            return prePlay;
        }
    }

    if(stoneCount >= MAX_STONES) {
        return WALL;
    } else {
        return NONE;
    }
}


int getPartLink(int putId, int searchDirect) {
    STONE_TYPE judgeType = allStones[putId].getType();

    if(judgeType == NONE || judgeType == WALL) {
        return 0;
    }

    int searchId = putId;
    int linkCount = 0;

    while(true) {
        searchId += searchDirect;
        STONE_TYPE curType = allStones[searchId].getType();

        if(curType == judgeType) {
            linkCount++;
        } else {
            return linkCount;
        }
    }
}

//for debug
void showTable() {
    for(int i = 0; i < TOTAL_BLOCK; i++) {
        if(allStones[i].getType() < 2) {
            printf("[%d,%d] = %s\n",i / BOARD_COL, i % BOARD_COL, allStones[i].getType() ? "White" : "Black");
        }
    }
    printf("------------------------\n\n");
}



void printLoc(int step) {
    printf("[%d,%c]\n", 15 + WALL_SIZE - step / BOARD_COL, 'A' - WALL_SIZE + step % BOARD_COL);
}

int transLoc(int &row, int &col) {
     return (row + WALL_SIZE) * BOARD_COL + (col + WALL_SIZE);
}

void transLoc(int step, int &row, int &col) {
    row = step / BOARD_COL - WALL_SIZE;
    col = step % BOARD_COL - WALL_SIZE;
}

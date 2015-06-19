#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdlib.h>

const int WALL_SIZE = 1;
const int MAX_STONES = 15 * 15;
const int BOARD_COL = 15 + WALL_SIZE * 2;
const int BOARD_ROW = 15 + WALL_SIZE * 2;
const int OFFSET = 1;
const int WIN_TRIGGER = 5;
const int searchGroup[4] = {1, BOARD_COL, BOARD_COL + 1, BOARD_COL - 1}; //{-, |, \, /}

int getStoneId(int row, int col);
void putOn(int type, int row, int col);
void putOn(int row, int col);
void printBoard();
void resetBoard();
int winCheck(int row, int col);
int getPutCount();

enum {NONE, BLACK, WHITE, WALL};
extern int board[BOARD_COL * BOARD_ROW];
#endif // BOARD_H_INCLUDED

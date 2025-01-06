#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>

extern bool hasVibrated;
extern int prevHoveredRow;
extern int prevHoveredCol;

void LoadAssets();
void FreeAssets();

void DrawObjects();

void DrawBoard(char board[3][3]);
void DrawCursor(int x, int y, uint32_t color);

#endif

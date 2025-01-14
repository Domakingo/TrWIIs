#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

extern bool highlightWinningCells;
extern Position winningPositions[3];

void LoadAssets();
void FreeAssets();
void DrawObjects();
void DrawBoard(char board[3][3]);
void DrawCursor(int x, int y, Player* player, GRRLIB_texImg* cursorTex);
void DrawWinningCells();
void ResetBoard();
void HandleDraw();

#endif


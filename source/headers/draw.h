#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

extern bool highlightWinningCells;
extern bool showDrawScore;
extern Position winningPositions[3];

void LoadAssets();
GRRLIB_texImg* CreateSubTexture(GRRLIB_texImg* sourceTex, int index);
void FreeAssets();
void DrawObjects();
void DrawBoard(char board[3][3]);
void DrawScore(int score1, int score2);
void DrawCursor(int x, int y, Player* player, GRRLIB_texImg* cursorTex);
void DrawWinningCells();
void ResetBoard();
void HandleDraw();

#endif


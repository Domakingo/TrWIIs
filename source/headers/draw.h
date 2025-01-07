#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "types.h"

extern bool hasVibrated;
extern int prevHoveredRow;
extern int prevHoveredCol;
extern bool isAnimating;
extern clock_t animationStartTime;

void LoadAssets();
void FreeAssets();

void DrawObjects();

void DrawBoard(char board[3][3]);
void DrawCursor(int x, int y, uint32_t color);

void HighlightWinningCells(Position winningPositions[3]);
void StartWinningAnimation(Position winningPositions[3]);
void HandleDraw();

#endif

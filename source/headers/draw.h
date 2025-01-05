#ifndef DRAW_H
#define DRAW_H

void LoadAssets();
void FreeAssets();

void DrawObjects();

void DrawBoard(char board[3][3]);
void DrawCursor(int x, int y, uint32_t color);

#endif

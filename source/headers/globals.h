#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

// Screen
extern const int screenWidth;
extern const int screenHeight;

// Board
extern char board[3][3];
extern uint32_t hoveredCellColor;

// Board position and size
extern const int gridStartX;
extern const int gridStartY;

extern const int gridSize;
extern const int cellSize;

// Thickness of the lines
extern const unsigned short boardThickness;
extern const unsigned short checksThickness;


// Functions

#endif
#ifndef GLOBALS_H
#define GLOBALS_H

#include <wiiuse/wpad.h>

// Players
typedef struct {
    ushort id;
    ir_t ir;
    char check;
    uint32_t color;
    bool myTurn;
} Player;

extern Player p1;
extern Player p2;

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
extern const ushort boardThickness;
extern const ushort checksThickness;


// Functions
extern Player currentPlayer();

#endif

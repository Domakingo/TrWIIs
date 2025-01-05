#include "headers/globals.h"

// Define the players
Player p1 = {.id = 0, .check = 'X', .color = 0x2CE8F5FF, .myTurn = true};
Player p2 = {.id = 1, .check = 'O', .color = 0xFF0044FF, .myTurn = false};

// Define screen dimensions
const int screenWidth = 640;
const int screenHeight = 480;

// Define the game board
char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
uint32_t hoveredCellColor = 0xFFFF0080;

// Define board position and size
const int gridStartX = 170;
const int gridStartY = 90;

const int gridSize = 300;
const int cellSize = 100;

// Thickness of the lines
const ushort boardThickness = 3;
const ushort checksThickness = 5;


// Functions
// Return the current player
Player currentPlayer() {
    return (p1.myTurn) ? p1 : p2;
}

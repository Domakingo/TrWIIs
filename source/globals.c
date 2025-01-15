#include "headers/globals.h"

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
const unsigned short boardThickness = 3;
const unsigned short marksThickness = 5;

int playedGames = 0;
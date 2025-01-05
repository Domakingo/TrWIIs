#include <grrlib.h>

#include "headers/assets.h"
#include "headers/globals.h"
#include "headers/draw.h"
#include "headers/input.h"
#include "headers/debug.h"

void LoadAssets() {
    // Load the images
    backgroundTex = GRRLIB_LoadTexture(background_jpg);
    if (backgroundTex == NULL) {
        debug_send("Failed to load background.jpg\n");
    }
    
    gridTex = GRRLIB_LoadTexture(grid_png);
    if (gridTex == NULL) {
        debug_send("Failed to load grid.png\n");
    }
}

void FreeAssets() {
    // Free the textures
    GRRLIB_FreeTexture(backgroundTex);
    GRRLIB_FreeTexture(gridTex);
}

void DrawObjects() {
    // Clear the screen with black
    GRRLIB_FillScreen(0x000000FF);

    // Draw the background image
    GRRLIB_DrawImg(0, 0, backgroundTex, 0, 1, 1, 0xFFFFFFFF);

    // Draw the grid image
    GRRLIB_DrawImg(gridStartX, gridStartY, gridTex, 0, 1, 1, 0xFFFFFFFF);

    // Draw the Tic-Tac-Toe board
    DrawBoard(board);

    // Draw the cursor
    DrawCursor(p1.ir.sx, p1.ir.sy, p1.color);
    DrawCursor(p2.ir.sx, p2.ir.sy, p2.color);

    // Render the frame buffer to the TV
    GRRLIB_Render();
}

void DrawBoard(char board[3][3]) {
    Player current = currentPlayer();

    // Check if the cursor is within the grid boundaries
    if (current.ir.sx >= gridStartX && current.ir.sx < gridStartX + gridSize &&
        current.ir.sy >= gridStartY && current.ir.sy < gridStartY + gridSize) {
        
        // Calculate the hovered column and row based on the cursor position
        int hoveredCol = (current.ir.sx - gridStartX) / cellSize;
        int hoveredRow = (current.ir.sy - gridStartY) / cellSize;

        // Check if the hovered cell is empty
        if (board[hoveredRow][hoveredCol] == ' ') {
            // Draw the hovered cell with the specified color
            GRRLIB_Rectangle(gridStartX + hoveredCol * cellSize, gridStartY + hoveredRow * cellSize, cellSize, cellSize, hoveredCellColor, true);
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X') {
                for (int t = 0; t < checksThickness; t++) {
                    GRRLIB_Line(gridStartX + cellSize * j + t, gridStartY + cellSize * i, gridStartX + cellSize * (j + 1) + t, gridStartY + cellSize * (i + 1), p1.color);
                    GRRLIB_Line(gridStartX + cellSize * (j + 1) + t, gridStartY + cellSize * i, gridStartX + cellSize * j + t, gridStartY + cellSize * (i + 1), p1.color);
                }
            } else if (board[i][j] == 'O') {
                for (int t = 0; t < checksThickness; t++) {
                    GRRLIB_Circle(gridStartX + cellSize * j + cellSize / 2, gridStartY + cellSize * i + cellSize / 2, (cellSize / 2) - t, p2.color, false);
                }
            }
        }
    }
}

void DrawCursor(int x, int y, uint32_t color) {
    GRRLIB_Circle(x, y, 10, color, true);
}
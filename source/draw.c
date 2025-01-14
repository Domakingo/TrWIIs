#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include <math.h>

#include "headers/draw.h"
#include "headers/player.h"
#include "headers/assets.h"
#include "headers/globals.h"
#include "headers/debug.h"
#include "headers/draw.h"

Position winningPositions[3];
bool highlightWinningCells = false;

void LoadAssets() {
    // Load the images
    backgroundTex = GRRLIB_LoadTexture(background_png);
    if (backgroundTex == NULL) {
        debug_send("Failed to load background.png\n");
    }
    
    gridTex = GRRLIB_LoadTexture(grid_png);
    if (gridTex == NULL) {
        debug_send("Failed to load grid.png\n");
    }

    XTex = GRRLIB_LoadTexture(X_png);
    if (XTex == NULL) {
        debug_send("Failed to load XTex.png\n");
    }

    OTex = GRRLIB_LoadTexture(O_png);
    if (OTex == NULL) {
        debug_send("Failed to load OTex.png\n");
    }

    p1CursorTex = GRRLIB_LoadTexture(p1Cursor_png);
    if (p1CursorTex == NULL) {
        debug_send("Failed to load p1CursorTex.png\n");
    }

    p2CursorTex = GRRLIB_LoadTexture(p2Cursor_png);
    if (p2CursorTex == NULL) {
        debug_send("Failed to load p2CursorTex.png\n");
    }
}

void FreeAssets() {
    GRRLIB_FreeTexture(backgroundTex);
    GRRLIB_FreeTexture(gridTex);
    GRRLIB_FreeTexture(XTex);
    GRRLIB_FreeTexture(OTex);
    GRRLIB_FreeTexture(p1CursorTex);
    GRRLIB_FreeTexture(p2CursorTex);
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

    // Draw the cursors
    DrawCursor(p1.ir.sx, p1.ir.sy, &p1, p1CursorTex);
    DrawCursor(p2.ir.sx, p2.ir.sy, &p2, p2CursorTex);

    // Render the frame buffer to the TV
    GRRLIB_Render();
}

void DrawBoard(char board[3][3]) {
    Player* current = currentPlayer();

    if (current != NULL) {
        // Check if the cursor is within the grid boundaries
        if (current->ir.sx >= gridStartX && current->ir.sx < gridStartX + gridSize &&
            current->ir.sy - 34 >= gridStartY && current->ir.sy - 34 < gridStartY + gridSize) {
            
            // Calculate the hovered column and row based on the cursor position
            int hoveredCol = (current->ir.sx - gridStartX) / cellSize;
            int hoveredRow = (current->ir.sy - 34 - gridStartY) / cellSize;

            // Check if the hovered cell is empty
            if (board[hoveredRow][hoveredCol] == ' ') {
                // Draw the hovered cell with the specified color
                GRRLIB_Rectangle(gridStartX + hoveredCol * cellSize, gridStartY + hoveredRow * cellSize, cellSize, cellSize, hoveredCellColor, true);
            }
        }
    }

    if (highlightWinningCells) {
        DrawWinningCells();
    }

    // Draw the marks on the board using textures
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int x = gridStartX + j * cellSize + (cellSize - 100) / 2; // Center the texture horizontally
            int y = gridStartY + i * cellSize + (cellSize - 100) / 2; // Center the texture vertically

            if (board[i][j] == 'X') {
                GRRLIB_DrawImg(x, y, XTex, 0, 1, 1, 0xFFFFFFFF); // Draw the X texture
            } else if (board[i][j] == 'O') {
                GRRLIB_DrawImg(x, y, OTex, 0, 1, 1, 0xFFFFFFFF); // Draw the O texture
            }
        }
    }
}

void DrawCursor(int x, int y, Player* player, GRRLIB_texImg* cursorTex) {
    WPADData* data = WPAD_Data(player->id);
    float angle = data->orient.roll;

    // Calculate the position to draw the image such that its center is at (x, y)
    int drawX = x - cursorTex->w / 2;
    int drawY = y - cursorTex->h / 2;

    // Draw the cursor image with rotation around its center
    GRRLIB_DrawImg(drawX, drawY, cursorTex, angle, 1, 1, 0xFFFFFFFF);
}

void DrawWinningCells() {
    for (int i = 0; i < 3; i++) {
        GRRLIB_Rectangle(gridStartX + winningPositions[i].col * cellSize, gridStartY + winningPositions[i].row * cellSize, cellSize, cellSize, 0xB8FCE2AA, true);
    }
}

void HandleDraw() {
    
}

void ResetBoard() {
    debug_send("Resetting board.\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
    p1.myTurn = true;
    p2.myTurn = false;
}
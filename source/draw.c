#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "headers/draw.h"
#include "headers/player.h"
#include "headers/assets.h"
#include "headers/globals.h"
#include "headers/debug.h"

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

    // p1 textures
    p1CursorTex[0] = GRRLIB_LoadTexture(p1Move_png);
    if (p1CursorTex[0] == NULL) {
        debug_send("Failed to load p1 move.png\n");
    }

    p1CursorTex[1] = GRRLIB_LoadTexture(p1Select_png);
    if (p1CursorTex[1] == NULL) {
        debug_send("Failed to load p1 select.png\n");
    }

    p1CursorTex[2] = GRRLIB_LoadTexture(p1Win_png);
    if (p1CursorTex[2] == NULL) {
        debug_send("Failed to load p1 win.png\n");
    }

    p1CursorTex[3] = GRRLIB_LoadTexture(p1Draw_png);
    if (p1CursorTex[3] == NULL) {
        debug_send("Failed to load p1 draw.png\n");
    }

    // p2 textures
    p2CursorTex[0] = GRRLIB_LoadTexture(p2Move_png);
    if (p2CursorTex[0] == NULL) {
        debug_send("Failed to load p2 move.png\n");
    }

    p2CursorTex[1] = GRRLIB_LoadTexture(p2Select_png);
    if (p2CursorTex[1] == NULL) {
        debug_send("Failed to load p2 select.png\n");
    }

    p2CursorTex[2] = GRRLIB_LoadTexture(p2Win_png);
    if (p2CursorTex[2] == NULL) {
        debug_send("Failed to load p2 win.png\n");
    }

    p2CursorTex[3] = GRRLIB_LoadTexture(p2Draw_png);
    if (p2CursorTex[3] == NULL) {
        debug_send("Failed to load p2 draw.png\n");
    }
}

void FreeAssets() {
    GRRLIB_FreeTexture(backgroundTex);
    GRRLIB_FreeTexture(gridTex);
    GRRLIB_FreeTexture(XTex);
    GRRLIB_FreeTexture(OTex);

    for (int i = 0; i < 4; i++){
        GRRLIB_FreeTexture(p1CursorTex[i]);
        GRRLIB_FreeTexture(p2CursorTex[i]);
    }
}

void DrawObjects() {
    // Clear the screen with black
    GRRLIB_FillScreen(0x000000FF);

    // Draw the background image
    if (backgroundTex != NULL) {
        GRRLIB_DrawImg(0, 0, backgroundTex, 0, 1, 1, 0xFFFFFFFF);
    }

    // Draw the grid image
    if (gridTex != NULL) {
        GRRLIB_DrawImg(gridStartX, gridStartY, gridTex, 0, 1, 1, 0xFFFFFFFF);
    }

    // Draw the Tic-Tac-Toe board
    DrawBoard(board);

    // Draw the cursors
    DrawCursor(p1.ir.sx, p1.ir.sy, &p1, p1CursorTex[p1.cursorStatus]);
    DrawCursor(p2.ir.sx, p2.ir.sy, &p2, p2CursorTex[p2.cursorStatus]);

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
                current->cursorStatus = 1;

                // Draw the hovered cell with the specified color
                GRRLIB_Rectangle(gridStartX + hoveredCol * cellSize, gridStartY + hoveredRow * cellSize, cellSize, cellSize, hoveredCellColor, true);
            } else {
                current->cursorStatus = 0;
            }
        } else {
            current->cursorStatus = 0;
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

            if (board[i][j] == 'X' && XTex != NULL) {
                GRRLIB_DrawImg(x, y, XTex, 0, 1, 1, 0xFFFFFFFF); // Draw the X texture
            } else if (board[i][j] == 'O' && OTex != NULL) {
                GRRLIB_DrawImg(x, y, OTex, 0, 1, 1, 0xFFFFFFFF); // Draw the O texture
            }
        }
    }
}

void DrawCursor(int x, int y, Player* player, GRRLIB_texImg* cursorTex) {
    if (cursorTex == NULL) {
        return;
    }

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

void ResetBoard() {
    debug_send("Resetting board.\n");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

    playedGames++;

    if(playedGames % 2 == 0) {
        p1.myTurn = true;
    } else {
        p2.myTurn = true;
    }

    p1.cursorStatus = 0;
    p2.cursorStatus = 0;
}
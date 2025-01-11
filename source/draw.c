#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp.h>
#include <unistd.h>

#include "headers/draw.h"
#include "headers/players.h"
#include "headers/assets.h"
#include "headers/globals.h"
#include "headers/debug.h"

bool hasVibrated = false;
int prevHoveredRow = -1;
int prevHoveredCol = -1;
bool isAnimating = false;
lwp_t animationThread;
Position winningPositions[3];

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
    Player* current = currentPlayer();

    // Check if the cursor is within the grid boundaries
    if (current != NULL && current->ir.sx >= gridStartX && current->ir.sx < gridStartX + gridSize &&
        current->ir.sy >= gridStartY && current->ir.sy < gridStartY + gridSize) {
        
        // Calculate the hovered column and row based on the cursor position
        int hoveredCol = (current->ir.sx - gridStartX) / cellSize;
        int hoveredRow = (current->ir.sy - gridStartY) / cellSize;

        // Check if the hovered cell is empty
        if (board[hoveredRow][hoveredCol] == ' ') {
            // Draw the hovered cell with the specified color
            GRRLIB_Rectangle(gridStartX + hoveredCol * cellSize, gridStartY + hoveredRow * cellSize, cellSize, cellSize, hoveredCellColor, true);

            // Make the controller vibrate if the hovered cell has changed
            if (hoveredRow != prevHoveredRow || hoveredCol != prevHoveredCol) {
                ActivateRumbleAsync(current, 1);
                prevHoveredRow = hoveredRow;
                prevHoveredCol = hoveredCol;
            }
        }
    } else {
        // Reset the previously hovered cell if the cursor is out of bounds
        prevHoveredRow = -1;
        prevHoveredCol = -1;
    }

    // Draw the marks on the board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X') {
                for (int t = 0; t < marksThickness; t++) {
                    GRRLIB_Line(gridStartX + cellSize * j + t, gridStartY + cellSize * i, gridStartX + cellSize * (j + 1) + t, gridStartY + cellSize * (i + 1), p1.color);
                    GRRLIB_Line(gridStartX + cellSize * (j + 1) + t, gridStartY + cellSize * i, gridStartX + cellSize * j + t, gridStartY + cellSize * (i + 1), p1.color);
                }
            } else if (board[i][j] == 'O') {
                for (int t = 0; t < marksThickness; t++) {
                    GRRLIB_Circle(gridStartX + cellSize * j + cellSize / 2, gridStartY + cellSize * i + cellSize / 2, (cellSize / 2) - t, p2.color, false);
                }
            }
        }
    }

    // If animating, highlight the winning cells
    if (isAnimating) {
        HighlightWinningCells(winningPositions);
    }
}

void DrawCursor(int x, int y, uint32_t color) {
    GRRLIB_Circle(x, y, 10, color, true);
}

static void* AnimationTask(void* arg) {
    debug_send("Animation task started.\n");
    for (int i = 0; i < 3; i++) {
        debug_send("Animation cycle %d\n", i);
        isAnimating = true;
        usleep(10000);
    }
    isAnimating = false;
    ResetBoard();
    debug_send("Animation task finished, board reset.\n");
    return NULL;
}

void HighlightWinningCells(Position winningPositions[3]) {
    if (!isAnimating) return;

    // Alternate color every 0.5 seconds with transparency
    static bool toggle = false;
    toggle = !toggle;
    uint32_t color = toggle ? 0x2CE8F5AA : 0x00000000; // Azzurro trasparente

    for (int i = 0; i < 3; i++) {
        int row = winningPositions[i].row;
        int col = winningPositions[i].col;
        GRRLIB_Rectangle(gridStartX + col * cellSize, gridStartY + row * cellSize, cellSize, cellSize, color, true);
    }
}

void StartWinningAnimation(Position winningPositions[3]) {
    debug_send("Starting winning animation.\n");
    for (int i = 0; i < 3; i++) {
        winningPositions[i] = winningPositions[i];
    }
    if (LWP_CreateThread(&animationThread, AnimationTask, NULL, NULL, 0, 80) != 0) {
        debug_send("Error: Could not create animation thread\n");
    }
}

void StartDrawAnimation() {
    debug_send("Starting draw animation.\n");
    if (LWP_CreateThread(&animationThread, AnimationTask, NULL, NULL, 0, 80) != 0) {
        debug_send("Error: Could not create animation thread\n");
    }
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
    prevHoveredRow = -1;
    prevHoveredCol = -1;
    hasVibrated = false;
}

void HandleDraw() {
    // Currently, do nothing for a draw
    // You can add any draw-specific logic here if needed
}
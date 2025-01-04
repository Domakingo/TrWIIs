#include <grrlib.h>

#include "headers/draw.h"
#include "headers/input.h"

void DrawObjects() {
    // Clear the screen with black
    GRRLIB_FillScreen(0x000000FF);

    // Draw the Tic-Tac-Toe board
    extern char board[3][3];  // Declare the board as extern to use it here
    DrawBoard(board);

    // Draw the cursor
    DrawCursor(ir1.sx, ir1.sy);

    // Render the frame buffer to the TV
    GRRLIB_Render();
}

void DrawBoard(char board[3][3]) {
    // Draw the grid
    for (int i = 1; i < 3; i++) {
        GRRLIB_Line(100 * i, 0, 100 * i, 300, RGBA(255, 255, 255, 255));  // Vertical lines
        GRRLIB_Line(0, 100 * i, 300, 100 * i, RGBA(255, 255, 255, 255));  // Horizontal lines
    }

    // Draw the marks
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X') {
                // Draw X
                GRRLIB_Line(100 * j, 100 * i, 100 * (j + 1), 100 * (i + 1), RGBA(255, 0, 0, 255));
                GRRLIB_Line(100 * (j + 1), 100 * i, 100 * j, 100 * (i + 1), RGBA(255, 0, 0, 255));
            } else if (board[i][j] == 'O') {
                // Draw O
                GRRLIB_Circle(100 * j + 50, 100 * i + 50, 40, RGBA(0, 0, 255, 255), false);
            }
        }
    }
}

void DrawCursor(int x, int y) {
    // Disegna un piccolo cerchio giallo dove il Wii Remote sta puntando
    GRRLIB_Circle(x, y, 5, RGBA(255, 255, 0, 255), true);
}
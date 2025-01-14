#include <wiiuse/wpad.h>
#include <stdlib.h>

#include "headers/input.h"
#include "headers/globals.h"
#include "headers/player.h"
#include "headers/update.h"
#include "headers/debug.h"

void HandleInput() {
    Player* current = currentPlayer();

    WPAD_ScanPads();

    // Update IR data
    WPAD_IR(WPAD_CHAN_0, &p1.ir);
    WPAD_IR(WPAD_CHAN_1, &p2.ir);

    // Check if currentPlayer() returns NULL
    if (current != NULL) {
        // If [HOME] was pressed on the first Wiimote, exit the game
        if (WPAD_ButtonsDown(current->id) & WPAD_BUTTON_HOME) {
            exit(0);
        }
    } else {
        // If [HOME] was pressed on the first Wiimote, exit the game
        if (WPAD_ButtonsDown(getPlayer(0)->id) & WPAD_BUTTON_HOME) {
            exit(0);
        }
    }

    if (current != NULL) {
         // Handle input for players
        if (WPAD_ButtonsDown(current->id) & WPAD_BUTTON_A) {
            // Calculate the grid index based on the cursor position
            int col = (current->ir.sx - gridStartX) / cellSize;
            int row = (current->ir.sy - 34 - gridStartY) / cellSize;

            // Ensure the calculated position is within grid bounds
            if (row >= 0 && row < 3 && col >= 0 && col < 3) {
                // Ensure the cursor is within the grid area
                if (current->ir.sx >= gridStartX && current->ir.sx < gridStartX + gridSize &&
                    current->ir.sy - 34 >= gridStartY && current->ir.sy - 34 < gridStartY + gridSize) {
                    PlaceMark(row, col);
                }
            }
        }
    }
}
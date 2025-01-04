#include <wiiuse/wpad.h>
#include <stdlib.h>

#include "headers/input.h"
#include "headers/update.h"

void HandleInput() {
    WPAD_ScanPads();  // Scan the Wiimotes

    // Update IR data
    WPAD_IR(WPAD_CHAN_0, &ir1);

    // If [HOME] was pressed on the first Wiimote, exit the game
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
        exit(0);
    }

    // Handle other input for Tic-Tac-Toe
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A) {
        // Calculate the grid index based on the cursor position
        int row = ir1.sx / 100;
        int col = ir1.sy / 100;

        // Place the symbol in the selected cell
        PlaceMark(row, col);
    }
}
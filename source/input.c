#include <wiiuse/wpad.h>
#include <stdlib.h>

#include "headers/input.h"

void HandleInput() {
    WPAD_ScanPads();  // Scan the Wiimotes

    // If [HOME] was pressed on the first Wiimote, break out of the loop
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
        exit(0);
    }
}

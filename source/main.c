#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"

// Define ir1 globally
ir_t ir1;

int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wiimotes
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    // Start the game loop
    GameLoop();

    // Clear the memory allocated by GRRLIB
    GRRLIB_Exit();

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"
#include "headers/debug.h"

int main(int argc, char **argv) {
    // Initialise debug network
    debug_init("192.168.1.73", 8000);
    debug_send("------------------------------------------------------------------------------------\n");

    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wiimotes
    WPAD_Init();

    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    WPAD_SetDataFormat(WPAD_CHAN_1, WPAD_FMT_BTNS_ACC_IR);

    // Load the assets
    LoadAssets();

    // Start the game loop
    GameLoop();

    // Clear the memory allocated by GRRLIB
    GRRLIB_Exit();

    // Free the assets
    FreeAssets();

    // Close the debug network
    debug_close();

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

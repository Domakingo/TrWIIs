#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"

int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wiimotes
    WPAD_Init();

    // Start the game loop
    GameLoop();

    // Clear the memory allocated by GRRLIB
    GRRLIB_Exit();

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

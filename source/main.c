/*===========================================
        GRRLIB (GX Version)
        - Minimal Example -

        Draw a Green Square
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <wiiuse/wpad.h>

int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wiimotes
    WPAD_Init();

    // Define the color green using the RGBA macro
    u32 green = RGBA(0, 255, 0, 255);  // RGBA for green

    // Loop forever
    while(1) {

        WPAD_ScanPads();  // Scan the Wiimotes

        // If [HOME] was pressed on the first Wiimote, break out of the loop
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;

        // Clear the screen with black
        GRRLIB_FillScreen(0x000000FF);

        // Draw a green square at position (100, 100) with size 50x50
        GRRLIB_Rectangle(100, 100, 50, 50, green, true);

        // Render the frame buffer to the TV
        GRRLIB_Render();
    }

    // Clear the memory allocated by GRRLIB
    GRRLIB_Exit();

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

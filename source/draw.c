#include <grrlib.h>

#include "headers/draw.h"

void DrawObjects() {
    u32 green = RGBA(0, 255, 0, 255);

    // Clear the screen with black
    GRRLIB_FillScreen(0x000000FF);

    // Draw a green square at position (100, 100) with size 50x50
    GRRLIB_Rectangle(100, 100, 50, 50, green, true);

    // Render the frame buffer to the TV
    GRRLIB_Render();
}

#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"
#include "headers/update.h"

void GameLoop() {
    while(1) {
        HandleInput();  // Handle user input
        UpdateObjects();  // Update game objects
        DrawObjects();  // Draw objects on the screen
    }
}

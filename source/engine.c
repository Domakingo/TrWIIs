#include "headers/engine.h"

void GameLoop() {
    while(1) {
        HandleInput();  // Handle user input
        UpdateObjects();  // Update game objects
        DrawObjects();  // Draw objects on the screen
    }
}
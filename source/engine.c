#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"
#include "headers/update.h"

void GameLoop() {
    while(1) {
        HandleInput();
        UpdateObjects();
        DrawObjects();
    }
}
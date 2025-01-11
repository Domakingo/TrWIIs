#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"
#include "headers/debug.h"

#include <unistd.h>

void GameLoop() {
    while(1) {
        HandleInput();
        DrawObjects();
    }
}
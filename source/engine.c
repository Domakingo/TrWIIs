#include "headers/engine.h"
#include "headers/input.h"
#include "headers/draw.h"
#include "headers/update.h"
#include "headers/globals.h"

#include <unistd.h>

void GameLoop() {
    while(1) {
        HandleInput();
        UpdateObjects();
        DrawObjects();
    }
}

void GameEnded(bool isDraw) {
    if (isDraw) {
        
        usleep(5000 * 1000);
    } else {
        
        usleep(5000 * 1000);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}
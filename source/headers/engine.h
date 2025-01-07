#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

#include "types.h"

extern Position winningPositions[3];

void GameLoop();
void GameEnded(bool isDraw);

#endif
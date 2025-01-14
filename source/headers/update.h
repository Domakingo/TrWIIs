#ifndef UPDATE_H
#define UPDATE_H

#include "player.h"
#include "types.h"

void PlaceMark(int row, int col);
bool CheckWinCondition(Player* player, Position winningPositions[3]);

#endif

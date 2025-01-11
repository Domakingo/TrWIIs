#ifndef UPDATE_H
#define UPDATE_H

#include "players.h"
#include "types.h"

extern Position winningPositions[3];

void PlaceMark(int row, int col);
bool CheckWinCondition(Player* player, Position winningPositions[3]);

#endif

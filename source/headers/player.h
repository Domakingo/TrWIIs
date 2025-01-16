#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

extern Player p1;
extern Player p2;

Player* currentPlayer();
Player* waitingPlayer();
Player* getPlayer(int id);
void ActivateRumbleAsync(Player* player, unsigned short millis);

#endif
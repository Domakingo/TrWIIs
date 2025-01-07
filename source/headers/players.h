#ifndef PLAYERS_H
#define PLAYERS_H

#include <wiiuse/wpad.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    unsigned short id;
    ir_t ir;
    char mark;
    uint32_t color;
    bool myTurn;
} Player;

extern Player p1;
extern Player p2;

Player* currentPlayer();
Player* waitingPlayer();
void ActivateRumble(Player* player, unsigned short millis);

#endif
#include "headers/players.h"

#include <unistd.h>

// Define the players
Player p1 = {.id = 0, .check = 'X', .color = 0x2CE8F5FF, .myTurn = true};
Player p2 = {.id = 1, .check = 'O', .color = 0xFF0044FF, .myTurn = false};

// Return the current player
Player currentPlayer() {
    return (p1.myTurn) ? p1 : p2;
}

// Activate the rumble feature for the specified player
void ActivateRumble(Player* player, unsigned short millis) {
    WPAD_Rumble(player->id, 1);
    usleep(millis * 1000);
    WPAD_Rumble(player->id, 0);
}
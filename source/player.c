#include "headers/player.h"
#include "headers/debug.h"

#include <stdlib.h>
#include <ogc/lwp.h>
#include <unistd.h>

// Define the players
Player p1 = {
    .id = 0,
    .mark = 'X',
    .color = 0x2CE8F5FF,
    .myTurn = true
};

Player p2 = {
    .id = 1,
    .mark = 'O',
    .color = 0xFF0044FF,
    .myTurn = false
};

// Return a pointer to the current player
Player* currentPlayer() {
    if(p1.myTurn == false && p2.myTurn == false) {
        return NULL;
    }
    
    return (p1.myTurn) ? &p1 : &p2;
}

// Return a pointer to the waiting player
Player* waitingPlayer() {
    return (p1.myTurn) ? &p2 : &p1;
}

Player* getPlayer(int id) {
    if (id == 0) {
        return &p1;
    }

    return &p2;
}

typedef struct {
    Player* player;
    unsigned short millis;
} RumbleTaskData;

static void* RumbleTask(void* arg) {
    RumbleTaskData* data = (RumbleTaskData*)arg;
    WPAD_Rumble(data->player->id, 1); // Turn on rumble

    usleep(data->millis * 1000);

    WPAD_Rumble(data->player->id, 0);

    free(data);
    return NULL;
}

void ActivateRumbleAsync(Player* player, unsigned short millis) {
    RumbleTaskData* data = (RumbleTaskData*)malloc(sizeof(RumbleTaskData));
    if (!data) {
        debug_send("Error: Could not allocate memory for RumbleTaskData\n");
        return;
    }
    data->player = player;
    data->millis = millis;

    lwp_t thread;
    if (LWP_CreateThread(&thread, RumbleTask, data, NULL, 0, 80) != 0) {
        debug_send("Error: Could not create rumble thread\n");
        free(data);
    }
}
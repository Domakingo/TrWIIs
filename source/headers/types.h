#ifndef TYPES_H
#define TYPES_H

#include <wiiuse/wpad.h>

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    uint8_t *buffer;
    uint32_t size;
    int voice;
    bool loop;
    bool autoFree;
} AudioAsset;

typedef struct Player {
    unsigned short id;
    ir_t ir;
    char mark;
    bool myTurn;
    int cursorStatus;
    int health;
    int hitHeight;
    int hitWidth;
} Player;

#endif

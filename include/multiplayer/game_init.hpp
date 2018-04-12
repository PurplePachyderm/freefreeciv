#pragma once
#include "../game/game.h"

//Events codes
#define M_MOVEMENT 1
#define M_ATTACK 2
#define M_CREATE_BARRACK 3
#define M_CREATE_PEASANT 4
#define M_CREATE_SOLDIER 5
#define M_HARVEST 6
#define M_END_TURN 7

//isAIControlled codes
#define AI 1
#define PLAYER 2

typedef struct{
    char * pseudo;
    int isAIControlled;
    int slot;
}mPlayer;

typedef struct{
    char * name;
    int nPlayers;
    mPlayer * players;
    struct struct game game;
}room;

typedef struct{
    int type;
    int unitId;
    coord target;
}event;

int wsConnect();

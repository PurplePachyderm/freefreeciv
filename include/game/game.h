#pragma once

#include "../coord.h"

        //Macros
    //Units types
#define PEASANT 0
#define SOLDIER 1

    //Building types
#define CITY 0
#define BARRACK 1

    //Player color/ID
#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3


    //Resources types
#define GOLD 0
#define WOOD 1

    //Map (TODO more sizes?)
#define MAP_SIZE 15

#define TURN_TIME 90 //sec

//Costs
#define PEASANT_COST 2
#define SOLDIER_COST 3
#define BARRACK_COST 5


        //Structures
    //Player
typedef struct{
    int type;   //See macros
    int owner; //== player.id
    coord pos;

    int life;
    int maxLife;

    int isBusy;    //Number of turns before being able to play
}building;


typedef struct{
    int type;
    int owner; //== player.id
    coord pos;

    int life;
    int maxLife;

    int attack;
    int isBusy;

    int movements;
    int maxMovements;
}unit;


typedef struct{
    int id;
    int isAIControlled;

    building * buildings;   //Array
    int nBuildings;
    unit * units;   //Array
    int nUnits;

    int gold;
    int wood;
}player;


    //Map
typedef struct{
    int type;
    coord pos;
}resource;


struct map{
    int size;

    resource * resources;   //Array
    int nResources;
};


    //Game (contains every info about the current game)
struct game{
    player * players;   //Array
    int nPlayers;
    int currentPlayer;

    struct struct map map;
};


    //Prototypes
int checkOwnBuilding(struct game, coord pos);
int checkOwnUnit(struct game, coord pos);

int checkForeignBuilding(struct game, coord pos, int * ownerId);
int checkForeignUnit(struct game, coord pos, int * ownerId);

void busyReset(struct struct game * game);

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
#define  GREEN 2
#define YELLOW 3


    //Resources types
#define GOLD 0
#define WOOD 1

    //Map (TODO more sizes?)
#define MAP_SIZE 15



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

    int movement;
    int maxMovement;
}unit;


typedef struct{
    int id;

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


typedef struct{
    int size;

    resource * resources;   //Array
    int nResources;
}map;


    //Game (contains every info about the current game)
typedef struct{
    player * players;   //Array
    int nPlayers;

    map map;
}game;

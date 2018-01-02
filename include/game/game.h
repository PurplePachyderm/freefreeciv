#pragma once

#include "../coord.h"

        //Macros
    //Units types
#define PEASANT 0;
#define SOLDIER 1;

    //Building types
#define CITY 0;
#define BARRACKS 1;

    //Resources types
#define GOLD 0;
#define WOOD 1;

    //Map (TODO more sizes)
#define MAP_SIZE 15



        //Structures
    //Player
typedef struct{
    int owner;  //== player.id
    int type;   //See macros
    coord pos;

    int life;
    int maxLife;

    int isBusy;    //Number of turns before being able to play
}building;


typedef struct{
    int owner;
    int type;
    coord pos;

    int life;
    int MaxLife;

    int attack;
    int isBusy;

    int speed;
    int maxSpeed;
}unit;


typedef struct{
    int id;

    building * buildings;   //Array
    unit * units;   //Array

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
}map;

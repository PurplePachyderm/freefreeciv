#include "../engine/engine.h"

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


        //Structures
    //Player
typedef struct{
    int owner;  //== player.id
    int type;   //See macros
    coord pos;

    int life;
}building;

typedef struct{
    int owner;
    int type;
    coord pos;

    int life;
    int attack;
    int speed;
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

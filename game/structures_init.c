#include "../include/coord.h"
#include "../include/game/game.h"

//Init functions put units/buildings in their states just after creation
//They also define their characteristics

void initPeasant(unit * peasant, int owner, int x, int y){
    peasant->type = PEASANT;
    peasant->owner = owner;

    peasant->coord.x = x;
    peasant->coord.y = y;

    peasant->life = 6;
    peasant->maxLife = 6;

    peasant->attack = 2;
    peasant->isBusy = 1;

    peasant->speed = 4;
    peasant->maxMovement = 4;
}



void initSoldier(unit * soldier, int owner, int x, int y){
    soldier->type = SOLDIER;
    soldier->owner = owner;

    soldier->coord.x = x;
    soldier->coord.y = y;

    soldier->life = 12;
    soldier->maxLife = 12;

    soldier->attack = 4;
    soldier->isBusy = 1;

    soldier->speed = 3;
    soldier->maxMovement = 3;
}



void initCity(building * city, int owner, int x, int y){
    city->type = CITY;
    city-> owner = owner;

    city->pos.x = x;
    city->pos.y = y;

    city->life = 15;
    city->maxLife = 15;

    city->isBusy = 0;   //Cities can't be created
}



void initBarrack(building * barrack, int owner, int x, int y){
    barrack->type = BARRACK;
    barrack-> owner = owner;

    barrack->pos.x = x;
    barrack->pos.y = y;

    barrack->life = 10;
    barrack->maxLife = 10;

    barrack->isBusy = 1;
}

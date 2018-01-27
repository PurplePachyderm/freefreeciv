#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/game/units_actions.h"
#include "../include/game/game.h"


int checkMap(game game, coord pos){
    //Iterates all game objects to check if a tile is free
    int tileIsOccupied = 0;

    for(int i=0; i<game.nPlayers; i++){
        for(int j=0; j<game.players[i].nUnits; j++){
            if(game.players[i].units[j].pos.x == pos.x && game.players[i].units[j].pos.y == pos.y)
                tileIsOccupied = 1;
        }

        for(int j=0; j<game.players[i].nBuildings; j++){
            if(game.players[i].buildings[j].pos.x == pos.x && game.players[i].buildings[j].pos.y == pos.y)
                tileIsOccupied = 1;
        }
    }

    for(int i=0; i<game.map.nResources; i++){
        if(game.map.resources[i].pos.x == pos.x && game.map.resources[i].pos.y == pos.y)
            tileIsOccupied = 1;
    }

    return tileIsOccupied;
}



void pathTesting(game game, coord startPos, coord targetPos, int size, coord * path, int currentIndex, int * pathFound){
    //This function will review all path possibilities of a given size to a target pos
    if(currentIndex<size && !*pathFound){
        coord newPos;

        for(int currentDir=1; currentDir<=4; currentDir++){ //Reviews all directions
            if(*pathFound)  //Avoids generating other path if one is good
                break;

            newPos.x = startPos.x;
            newPos.y = startPos.y;
            switch(currentDir){ //Determination of next tile
                case 1: //Up
                    newPos.y--;
                    break;

                case 2: //Right
                    newPos.x++;
                    break;

                case 3: //Down
                    newPos.y++;
                    break;

                case 4: //Left
                    newPos.x--;
            }

            int tileIsOccupied = checkMap(game, newPos);
            if(!tileIsOccupied){    //If the tile is occupied all "children" path won't be tested
                path[currentIndex].x = newPos.x;
                path[currentIndex].y = newPos.y;

                if(newPos.x == targetPos.x && newPos.y == targetPos.y)
                    *pathFound = 1; //All functions will be leaved

                else if(currentIndex != size-1)    //Continues to the next tile
                    pathTesting(game, newPos, targetPos, size, path, currentIndex+1, pathFound);
                //If none of these conditions are satisfied, a new direction will be tested
            }
        }
    }
}



int moveUnit(game * game, int ownerId, int unitId, coord targetPos, coord ** path){
    //Returns 0 if no pass has been found, otherwise returns its length
    coord startPos = game->players[ownerId].units[unitId].pos;
    int size = 0;

    int distX = abs(targetPos.x-game->players[ownerId].units[unitId].pos.x);
    int distY = abs(targetPos.y-game->players[ownerId].units[unitId].pos.y);
    int dist = distX+distY;

    if(dist<=game->players[ownerId].units[unitId].movements && !checkMap(*game, targetPos)){

        //Tests all path possibilities from shortest to longest
        for(int i=dist; i<=game->players[ownerId].units[unitId].movements; i++){
            *path = (coord *) malloc(i * sizeof(coord));

            int pathFound = 0;
            pathTesting(*game, startPos, targetPos, i, *path, 0, &pathFound);

            if(pathFound){
                size = i;
                break;
            }
        }
    }

    return size;
}



void collect(game * game, int ownerId, int unitId, coord targetPos){
    int distX = abs(targetPos.x-game->players[ownerId].units[unitId].pos.x);
    int distY = abs(targetPos.y-game->players[ownerId].units[unitId].pos.y);
    int dist = distX+distY;

    if(dist == 1){
        for(int i=0; i<game->map.nResources; i++){  //Looks for the targeted resources
            if(targetPos.x == game->map.resources[i].pos.x && targetPos.y == game->map.resources[i].pos.y){

                switch(game->map.resources[i].type){    //Updates player resources
                    case GOLD:
                        game->players[ownerId].gold++;
                        break;
                    case WOOD:
                        game->players[ownerId].wood++;
                }
{}
                game->players[ownerId].units[unitId].isBusy = 1;
                break;
            }
        }
    }
}



void attack(game * game, int ownerId, int unitId, coord targetPos){
    int distX = abs(targetPos.x-game->players[ownerId].units[unitId].pos.x);
    int distY = abs(targetPos.y-game->players[ownerId].units[unitId].pos.y);
    int dist = distX+distY;

    if(dist == 1){
        for(int i=0; i<game->nPlayers; i++){
            if(i != ownerId){

                for(int j=0; j<game->players[i].nUnits; j++){   //Cycles units
                    if(targetPos.x == game->players[i].units[j].pos.x && targetPos.y == game->players[i].units[j].pos.y){

                        game->players[i].units[j].life -= game->players[i].units[j].attack;

                        if(game->players[i].units[j].life <= 0){    //Death of unit
                            game->players[i].nUnits--;
                            unit * newUnits;  //Manual realloc of array
                            newUnits = (unit*) malloc (game->players[i].nUnits * sizeof(unit));

                            int id = 0; //We need a separate counter since it must not be incremented for the dead unit
                            for(int k=0; k<game->players[i].nUnits; k++){

                                if(k != unitId){
                                    newUnits[id].type = game->players[i].units[k].type;
                                    newUnits[id].owner = game->players[i].units[k].owner;

                                    newUnits[id].pos.x = game->players[i].units[k].pos.x;
                                    newUnits[id].pos.y = game->players[i].units[k].pos.y;

                                    newUnits[id].life = game->players[i].units[k].life;
                                    newUnits[id].maxLife = game->players[i].units[k].maxLife;

                                    newUnits[id].movements = game->players[i].units[k].movements;
                                    newUnits[id].maxMovements = game->players[i].units[k].maxMovements;

                                    newUnits[id].attack = game->players[i].units[k].attack;
                                    newUnits[id].isBusy = game->players[i].units[k].isBusy;

                                    id++;
                                }

                                free(game->players[i].units);
                                game->players[i].units = newUnits;
                            }
                        }
                        break;
                    }
                }

                for(int j=0; j<game->players[i].nBuildings; j++){
                    if(targetPos.x == game->players[i].buildings[j].pos.x && targetPos.y == game->players[i].buildings[j].pos.y){

                        game->players[i].buildings[j].life -= game->players[i].units[j].attack;

                        if(game->players[i].buildings[j].life <= 0){
                            //WIP
                        }
                    }
                }
            }
        }
    }
}

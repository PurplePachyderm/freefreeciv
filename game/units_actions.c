#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/game/units_actions.h"

//TODO Test those functions

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
    //Returns 0 if no pass has been found, otherwise returns its mength
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

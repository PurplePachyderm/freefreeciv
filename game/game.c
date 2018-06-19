#include <stdio.h>
#include <stdlib.h>

#include "../include/game/game.h"
#include "../include/coord.h"


 //Checks for the presence of one of current player's buildings on a tile
 //(returns size of buildings array if none has been found)
int checkOwnBuilding(struct game game, coord pos){
    int buildingFound = 0;
    int buildingId;

    for(int i=0; i<game.players[game.currentPlayer].nBuildings; i++){
        if(game.players[game.currentPlayer].buildings[i].pos.x == pos.x
        && game.players[game.currentPlayer].buildings[i].pos.y == pos.y){
            buildingFound = 1;
            buildingId = i;
            break;
        }
    }

    if(!buildingFound)
        buildingId = game.players[game.currentPlayer].nBuildings;

    return buildingId;
}


//Same for units
int checkOwnUnit(struct game game, coord pos){
    int unitFound = 0;
    int unitId;

    for(int i=0; i<game.players[game.currentPlayer].nUnits; i++){
        if(game.players[game.currentPlayer].units[i].pos.x == pos.x
        && game.players[game.currentPlayer].units[i].pos.y == pos.y){
            unitFound = 1;
            unitId = i;
            break;
        }
    }

    if(!unitFound)
        unitId = game.players[game.currentPlayer].nUnits;
    return unitId;
}


//Checks for foreign building on a tile (returns building id, ownerId is modified too)
//ownerId takes size of players array if none has been found
int checkForeignBuilding(struct game game, coord pos, int * ownerId){
    int buildingFound = 0;
    int buildingId;


    for(int i=0; i<game.nPlayers; i++){

        if(i != game.currentPlayer){
            for(int j=0; j<game.players[i].nBuildings; j++){

                if(game.players[i].buildings[j].pos.x == pos.x
                && game.players[i].buildings[j].pos.y == pos.y){
                    buildingFound = 1;
                    *ownerId = i;
                    buildingId = j;
                    break;
                }
            }
        }

        if(buildingFound)
            break;
    }

    if(!buildingFound){
        buildingId = 0;   //Not important if no building has been found
        *ownerId = game.nPlayers + 1;
    }


    return buildingId;
}


//Same for units
int checkForeignUnit(struct game game, coord pos, int * ownerId){
    int unitFound = 0;
    int unitId;


    for(int i=0; i<game.nPlayers; i++){

        if(i != game.currentPlayer){
            for(int j=0; j<game.players[i].nUnits; j++){

                if(game.players[i].units[j].pos.x == pos.x
                && game.players[i].units[j].pos.y == pos.y){
                    unitFound = 1;
                    *ownerId = i;
                    unitId = j;
                    break;
                }
            }
        }

        if(unitFound)
            break;
    }

    if(!unitFound){
        unitId = 0;   //Not important if no building has been found
        *ownerId = game.nPlayers + 1;
    }

    return unitId;
}



void busyReset(struct game * game){
    //Sets all isBusy to 0 (to be called at beginning of turn) & resets movements
    for(int i=0; i<game->nPlayers; i++){
        for(int j=0; j<game->players[i].nUnits; j++){
            game->players[i].units[j].isBusy = 0;
            game->players[i].units[j].movements = game->players[i].units[j].maxMovements;
        }

        for(int j=0; j<game->players[i].nBuildings; j++){
            game->players[i].buildings[j].isBusy = 0;
        }
    }
}



void freeGame(struct game * game){
    //Free tokens/players
    for(int i=0; i<game->nPlayers; i++){
        free(game->players[i].units);
        free(game->players[i].buildings);
    }

    free(game->players);

    //Free map
    free(game->map.resources);
}

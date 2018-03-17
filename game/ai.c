#include <stdio.h>
#include <stdlib.h>
#include "../include/game/ai.h"
#include "../include/game/game.h"
#include "../include/game/units_actions.h"
#include "../include/coord.h"


void initAI(game game, ai * ai){
    if(game.players[game.currentPlayer].nBuildings > 1)
       ai->hasBarrack = game.players[game.currentPlayer].nBuildings-1;  //Number of barrack
    else
       ai->hasBarrack = 0;

    ai->currentUnit = 0;
    ai->currentBuilding = 0;

    ai->nHarvestGold = 0;
    ai->nHarvestWood = 0;
    ai->nBuildBarrack = 0;
    ai->nTargetCity = 0;
    ai->nTargetUnit = 0;

    ai->movementTarget.x = 0;
    ai->movementTarget.y = 0;
    ai->actionTarget.x = 0;
    ai->actionTarget.y = 0;
}



int routineAI(game * game, ai * ai){
    //Determines the action for one unit/building
    //Returns the action Id (it will be executed in AIHud)

    int action = 0; //Return value
    coord enemyPos;

    //Plays with units first
    if(ai->currentUnit < game->players[game->currentPlayer].nUnits){

        switch(game->players[game->currentPlayer].units[ai->currentUnit].type){
            case PEASANT:
                if(!ai->hasBarrack
                || game->players[game->currentPlayer].wood + ai->nHarvestWood >= BARRACK_COST){
                //Construction of barrack is the prior task

                    //If it can be built
                    if(game->players[game->currentPlayer].wood >= BARRACK_COST){
                        ai->actionTarget = getTokenCreationPos(*game, game->players[game->currentPlayer].units[ai->currentUnit].pos);
                        ai->movementTarget = game->players[game->currentPlayer].units[ai->currentUnit].pos;
                        ai->nBuildBarrack++;
                        action = BUILDING_CREATION;
                    }
                    else{   //Harvest wood
                        int resourceId = findResource(game, ai->currentUnit, WOOD);

                        ai->actionTarget = game->map.resources[resourceId].pos;
                        ai->movementTarget = pathfindingAI(game, ai->currentUnit, game->map.resources[resourceId].pos, 1);
                        ai->nHarvestWood++;
                        action = HARVEST;
                    }
                }

                else if(enemyDistToCity(*game, &enemyPos) > 1){
                    //If no unit is attacking our city, peasant will harvest resources

                    int resourceType;
                    if(ai->nHarvestWood >= ai->nHarvestGold * (ai->hasBarrack+1)){  //Desired ratio between wood and goold harvesters
                        resourceType = GOLD;
                    }
                    else{
                        resourceType = WOOD;
                    }

                    int resourceId = findResource(game, ai->currentUnit, resourceType);

                    ai->actionTarget = game->map.resources[resourceId].pos;
                    ai->movementTarget = pathfindingAI(game, ai->currentUnit, game->map.resources[resourceId].pos, 1);
                    ai->nHarvestWood++;
                    action = HARVEST;

                }

                else{
                    //If an enemy unit is near the city, peasant will attack it
                    enemyDistToCity(*game, &enemyPos);
                    ai->actionTarget = enemyPos;
                    ai->movementTarget = pathfindingAI(game, ai->currentUnit, enemyPos, 1);
                    ai->nTargetUnit++;
                    action = ATTACK;

                }
                break;

            case SOLDIER:
                if(enemyDistToCity(*game, &enemyPos) > 1 && enemyDistToUnit(*game, *ai, &enemyPos) > 1){
                    //Attack closest enemy city
                    int enemyCityDist = 999;
                    coord enemyCityCoord;

                    //Looks for closest city
                    for(int i=0; i<game->nPlayers; i++){
                        if(i != game->currentPlayer){
                            int distX = abs(game->players[i].buildings[0].pos.x - game->players[game->currentPlayer].buildings[0].pos.x);
                            int distY = abs(game->players[i].buildings[0].pos.y - game->players[game->currentPlayer].buildings[0].pos.y);
                            int dist = distX + distY;

                            if(dist < enemyCityDist){
                                enemyCityDist = dist;
                                enemyCityCoord.x = game->players[i].buildings[0].pos.x;
                                enemyCityCoord.y = game->players[i].buildings[0].pos.y;
                            }
                        }
                    }

                    pathfindingAI(game, ai->currentUnit, enemyCityCoord, 1);
                    ai->actionTarget = enemyCityCoord;
                    ai->movementTarget = pathfindingAI(game, ai->currentUnit, enemyCityCoord, 1);
                    ai->nTargetCity++;
                    action = ATTACK;
                }


                else if(enemyDistToCity(*game, &enemyPos) == 1){
                    //Attack enemy close to our city
                    enemyDistToCity(*game, &enemyPos);
                    ai->actionTarget = enemyPos;
                    ai->movementTarget = pathfindingAI(game, ai->currentUnit, enemyPos, 1);
                    ai->nTargetCity++;
                    action = ATTACK;
                }


                else{
                    //Attack adjacent enemy unit
                    enemyDistToUnit(*game, *ai, &enemyPos);
                    ai->actionTarget = enemyPos;
                    ai->movementTarget = pathfindingAI(game, ai->currentUnit, enemyPos, 1);
                    ai->nTargetUnit++;
                    action = ATTACK;
                }

                break;
        }
    }


    else if(ai->currentBuilding < game->players[game->currentPlayer].nBuildings){   //Buildings
        switch(game->players[game->currentPlayer].buildings[ai->currentBuilding].type){
            case CITY:
                //Create peasant?
                if((!ai->hasBarrack || getNPeasants(*game) < getNSoldiers(*game)) && game->players[game->currentPlayer].gold > PEASANT_COST){
                    ai->actionTarget = getTokenCreationPos(*game, game->players[game->currentPlayer].buildings[ai->currentBuilding].pos);
                    action = UNIT_CREATION;
                }

                else{
                    action = PASS_TURN;
                }

                break;

            case BARRACK:
            //Create soldier?
            if(getNPeasants(*game) >= getNSoldiers(*game) && game->players[game->currentPlayer].gold > SOLDIER_COST){
                ai->actionTarget = getTokenCreationPos(*game, game->players[game->currentPlayer].buildings[ai->currentBuilding].pos);
                action = UNIT_CREATION;
            }

            else{
                action = PASS_TURN;
            }

            break;
        }
    }


    return action;
}



coord pathfindingAI(game * game, int unitId, coord targetPos, int tileIsOccupied){
    //Allows dynamic multi-turn pathfinding in a 2D environment using a particular case of Dijsktra's algorithm ;)
    //Returns the position the unit will have to move to for the current turn

    coord returnCoord;
    returnCoord.x = 0;
    returnCoord.y = 0;

    int originalMovCap = game->players[game->currentPlayer].units[unitId].movements;
    game->players[game->currentPlayer].units[unitId].movements = 999;   //Unlimited movement range is used to figure out entire path

    coord * path;
    int pathFound = moveUnit(game, unitId, targetPos, &path);

    if(pathFound){
        returnCoord = path[originalMovCap - 1];    //Tile to reach in one turn
    }
    else if(pathFound > 0 && pathFound <= originalMovCap && tileIsOccupied){
        //If we can directly reach the tile and it's supposed to be occupied, we stop the tile before
        returnCoord = path[originalMovCap - 2];
    }


    game->players[game->currentPlayer].units[unitId].movements = originalMovCap;

    free(path);

    return returnCoord;
}



int estimateTrueDist(game * game, int unitId, coord targetPos){
    //Returns the distance that a unit actually has to travel to reach its target
    //(using pathfinding)

    int originalMovCap = game->players[game->currentPlayer].units[unitId].movements;
    game->players[game->currentPlayer].units[unitId].movements = 999;

    coord * path;
    int trueDistance = moveUnit(game, unitId, targetPos, &path);

    game->players[game->currentPlayer].units[unitId].movements = originalMovCap;
    free(path);

    return trueDistance;
}



int findResource(game * game, int unitId, int resourceType){
    //Looks for closest resource

    int bestResource;
    int bestDist = 999;
    int dist;

    for(int i=0; i<game->map.nResources; i++){
        if(game->map.resources[i].type == resourceType){
            dist = estimateTrueDist(game, unitId, game->map.resources[i].pos);

            if(dist < bestDist){
                bestResource = i;
                bestDist = dist;
            }
        }
    }

    return bestResource;
}



int enemyDistToCity(game game, coord * enemyPos){
    //Scans all enemy units and returns the lowest distance to own city
    //Will allow us to know if a unit has to attack and who

    int lowestDist = 999;
    int distX;
    int distY;
    int dist;

    for(int i=0; i<game.nPlayers; i++){
        if(i != game.currentPlayer){
            for(int j=0; j<game.players[i].nUnits; j++){
                distX = abs(game.players[game.currentPlayer].buildings[0].pos.x
                - game.players[i].units[j].pos.x);

                distY = abs(game.players[game.currentPlayer].buildings[0].pos.y
                - game.players[i].units[j].pos.y);

                dist = distX + distY;

                if(dist < lowestDist){
                    lowestDist = dist;
                    *enemyPos = game.players[i].units[j].pos;
                }
            }
        }
    }

    return lowestDist;
}



int enemyDistToUnit(game game, ai ai, coord * enemyPos){
    //Scans all enemy units and returns the lowest distance to own city
    //Will allow us to know if a unit has to attack and who

    int lowestDist = 999;
    int distX;
    int distY;
    int dist;

    for(int i=0; i<game.nPlayers; i++){
        if(i != game.currentPlayer){
            for(int j=0; j<game.players[i].nUnits; j++){
                distX = abs(game.players[game.currentPlayer].units[ai.currentUnit].pos.x
                - game.players[i].units[j].pos.x);

                distY = abs(game.players[game.currentPlayer].units[ai.currentUnit].pos.y
                - game.players[i].units[j].pos.y);

                dist = distX + distY;

                if(dist < lowestDist){
                    lowestDist = dist;
                    *enemyPos = game.players[i].units[j].pos;
                }
            }
        }
    }

    return lowestDist;
}


coord getTokenCreationPos(game game, coord sourcePos){
    coord testPos;
    coord createPos;
    createPos.x = 0;
    createPos.y = 0;

    for(int i=1; i<4; i++){
        testPos = sourcePos;

        switch(i){
            case 1: //Up
                testPos.y--;
                break;

            case 2: //Right
                testPos.x++;
                break;

            case 3: //Down
                testPos.y++;
                break;

            case 4: //Left
                testPos.x--;
                break;
        }

        if(!checkMap(game, testPos) && testPos.x >= 1 && testPos .x <= MAP_SIZE && testPos.y >= 1 && testPos .y <= MAP_SIZE){
            createPos = testPos;
            break;
        }

    }

    return createPos;
}



int getNPeasants(game game){
    int nPeasants = 0;

    for(int i=0; i<game.players[game.currentPlayer].nUnits; i++){
        if(game.players[game.currentPlayer].units[i].type == PEASANT){
            nPeasants++;
        }
    }

    return nPeasants;
}



int getNSoldiers(game game){
    int nSoldiers = 0;

    for(int i=0; i<game.players[game.currentPlayer].nUnits; i++){
        if(game.players[game.currentPlayer].units[i].type == SOLDIER){
            nSoldiers++;
        }
    }

    return nSoldiers;
}

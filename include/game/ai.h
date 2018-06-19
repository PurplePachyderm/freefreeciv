#pragma once
#include "game.h"

    //Macros/Structure
//Exit code
#define ATTACK 1
#define HARVEST 2
#define UNIT_CREATION 3
#define BUILDING_CREATION 4
#define PASS_TURN 5
#define END_AI_TURN 6


//Contains all parameters used by the AI system for one turn
typedef struct{
    int hasBarrack;

    int currentUnit;
    int currentBuilding;

    //Number of peasants for each task
    int nHarvestWood;
    int nHarvestGold;
    int nBuildBarrack;

    //Number of units targetting each element
    int nTargetCity;
    int nTargetUnit;

    //Required data for current action
    coord movementTarget;
    coord actionTarget;

    //Number of tokens before playing
    int nBuildings;
    int nUnits;
}ai;

//Prototypes
void initAI(struct game game, ai * ai);
int routineAI(struct game * game, ai * ai);

coord pathfindingAI(struct game * game, int unitId, coord targetPos, int tileIsOccupied);
int estimateTrueDist(struct game * game, int unitId, coord targetPos);

int findResource(struct game * game, int unitId, int resourceType);
int enemyDistToCity(struct game game, coord * enemyPos);
int enemyDistToUnit(struct game game, ai ai, coord * enemyPos);

coord getTokenCreationPos(struct game game, coord sourcePos);

int getNPeasants(struct game game);
int getNSoldiers(struct game game);

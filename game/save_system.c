#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "../include/game/save_system.h"

//Déso pas déso ¯\_(ツ)_/¯

void createSave(char * saveName, game game){

        //Creation of JSON string
    json_object * jGame = json_object_new_object();

    //game.nPlayers
    json_object * jNPlayers = json_object_new_int(game.nPlayers);
    json_object_object_add(jGame, "nPlayers", jNPlayers);

    //game.players
    json_object **  jPlayers = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
    json_object *  jPlayersArray = json_object_new_array();

    json_object **  jPlayersId = (json_object**) malloc(game.nPlayers * sizeof(json_object*));

    json_object **  jPlayersNBuildings = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
    json_object *** jPlayersBuildings = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
    json_object **  jPlayersBuildingsArray = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
        json_object *** jBuildingsType = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jBuildingsOwner = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jBuildingsPos = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
            json_object *** jBuildingsX = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
            json_object *** jBuildingsY = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jBuildingsLife = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jBuildingsMaxLife = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jBuildingsIsBusy = (json_object***) malloc(game.nPlayers * sizeof(json_object**));

    json_object **  jPlayersNUnits = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
    json_object *** jPlayersUnits = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
    json_object **  jPlayersUnitsArray = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
        json_object *** jUnitsType = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsOwner = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsPos = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
            json_object *** jUnitsX = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
            json_object *** jUnitsY = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsLife = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsMaxLife = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsAttack = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsIsBusy = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsMovements = (json_object***) malloc(game.nPlayers * sizeof(json_object**));
        json_object *** jUnitsMaxMovements = (json_object***) malloc(game.nPlayers * sizeof(json_object**));

    json_object **  jPlayersGold = (json_object**) malloc(game.nPlayers * sizeof(json_object*));
    json_object **  jPlayersWood = (json_object**) malloc(game.nPlayers * sizeof(json_object*));

    for(int i=0; i<game.nPlayers; i++){
        jPlayers[i] = json_object_new_object();
        //player.id
        jPlayersId[i] = json_object_new_int(game.players[i].id);
        json_object_object_add(jPlayers[i], "id", jPlayersId[i]);

        //player.nBuildings
        jPlayersNBuildings[i] = json_object_new_int(game.players[i].nBuildings);
        json_object_object_add(jPlayers[i], "nBuildings", jPlayersNBuildings[i]);

        //player.buildings
        jPlayersBuildings[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
        jPlayersBuildingsArray[i] = json_object_new_array();
            jBuildingsType[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
            jBuildingsOwner[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
            jBuildingsPos[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
                jBuildingsX[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
                jBuildingsY[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
            jBuildingsLife[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
            jBuildingsMaxLife[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));
            jBuildingsIsBusy[i] = (json_object**) malloc(game.players[i].nBuildings * sizeof(json_object*));

            for(int j=0; j<game.players[i].nBuildings; j++){
                jPlayersBuildings[i][j] = json_object_new_object();

                //building.type
                jBuildingsType[i][j] = json_object_new_int(game.players[i].buildings[j].type);
                json_object_object_add(jPlayersBuildings[i][j], "type", jBuildingsType[i][j]);

                //building.owner
                jBuildingsOwner[i][j] = json_object_new_int(game.players[i].buildings[j].owner);
                json_object_object_add(jPlayersBuildings[i][j], "owner", jBuildingsOwner[i][j]);

                //building.pos
                jBuildingsPos[i][j] = json_object_new_object();

                jBuildingsX[i][j] = json_object_new_int(game.players[i].buildings[j].pos.x);
                json_object_object_add(jBuildingsPos[i][j], "x", jBuildingsX[i][j]);

                jBuildingsY[i][j] = json_object_new_int(game.players[i].buildings[j].pos.y);
                json_object_object_add(jBuildingsPos[i][j], "y", jBuildingsY[i][j]);

                json_object_object_add(jPlayersBuildings[i][j], "pos", jBuildingsPos[i][j]);

                //building.life
                jBuildingsLife[i][j] = json_object_new_int(game.players[i].buildings[j].life);
                json_object_object_add(jPlayersBuildings[i][j], "life", jBuildingsLife[i][j]);

                //building.maxLife
                jBuildingsMaxLife[i][j] = json_object_new_int(game.players[i].buildings[j].maxLife);
                json_object_object_add(jPlayersBuildings[i][j], "maxLife", jBuildingsMaxLife[i][j]);

                //building.isBusy
                jBuildingsIsBusy[i][j] = json_object_new_int(game.players[i].buildings[j].isBusy);
                json_object_object_add(jPlayersBuildings[i][j], "isBusy", jBuildingsIsBusy[i][j]);

                //Appending to array
                json_object_array_add(jPlayersBuildingsArray [i], jPlayersBuildings[i][j]);
            }
            json_object_object_add(jPlayers[i], "buildings", jPlayersBuildingsArray[i]);

            //player.nUnits
            jPlayersNUnits[i] = json_object_new_int(game.players[i].nUnits);
            json_object_object_add(jPlayers[i], "nUnits", jPlayersNUnits[i]);

            //player.units
            jPlayersUnits[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
            jPlayersUnitsArray[i] = json_object_new_array();
                jUnitsType[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsOwner[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsPos[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                    jUnitsX[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                    jUnitsY[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsLife[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsMaxLife[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsAttack[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsIsBusy[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsMovements[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));
                jUnitsMaxMovements[i] = (json_object**) malloc(game.players[i].nUnits * sizeof(json_object*));

            for(int j=0; j<game.players[i].nUnits; j++){
                jPlayersUnits[i][j] = json_object_new_object();

                //unit.type
                jUnitsType[i][j] = json_object_new_int(game.players[i].units[j].type);
                json_object_object_add(jPlayersUnits[i][j], "type", jUnitsType[i][j]);

                //unit.owner
                jUnitsOwner[i][j] = json_object_new_int(game.players[i].units[j].owner);
                json_object_object_add(jPlayersUnits[i][j], "owner", jUnitsOwner[i][j]);

                //unit.pos
                jUnitsPos[i][j] = json_object_new_object();

                jUnitsX[i][j] = json_object_new_int(game.players[i].units[j].pos.x);
                json_object_object_add(jUnitsPos[i][j], "x", jUnitsX[i][j]);

                jUnitsY[i][j] = json_object_new_int(game.players[i].units[j].pos.y);
                json_object_object_add(jUnitsPos[i][j], "y", jUnitsY[i][j]);

                json_object_object_add(jPlayersUnits[i][j], "pos", jUnitsPos[i][j]);

                //unit.life
                jUnitsLife[i][j] = json_object_new_int(game.players[i].units[j].life);
                json_object_object_add(jPlayersUnits[i][j], "life", jUnitsLife[i][j]);

                //unit.maxLife
                jUnitsMaxLife[i][j] = json_object_new_int(game.players[i].units[j].maxLife);
                json_object_object_add(jPlayersUnits[i][j], "maxLife", jUnitsMaxLife[i][j]);

                //unit.attack
                jUnitsAttack[i][j] = json_object_new_int(game.players[i].units[j].attack);
                json_object_object_add(jPlayersUnits[i][j], "attack", jUnitsAttack[i][j]);

                //unit.isBusy
                jUnitsIsBusy[i][j] = json_object_new_int(game.players[i].units[j].isBusy);
                json_object_object_add(jPlayersUnits[i][j], "isBusy", jUnitsIsBusy[i][j]);

                //unit.movements
                jUnitsMovements[i][j] = json_object_new_int(game.players[i].units[j].movements);
                json_object_object_add(jPlayersUnits[i][j], "movements", jUnitsMovements[i][j]);

                //unit.maxMovements
                jUnitsMaxMovements[i][j] = json_object_new_int(game.players[i].units[j].maxMovements);
                json_object_object_add(jPlayersUnits[i][j], "maxMovements", jUnitsMaxMovements[i][j]);

                json_object_array_add(jPlayersUnitsArray[i], jPlayersUnits[i][j]);
            }
            json_object_object_add(jPlayers[i], "units", jPlayersUnitsArray[i]);

            //player.gold
            jPlayersGold[i] = json_object_new_int(game.players[i].gold);
            json_object_object_add(jPlayers[i], "gold", jPlayersGold[i]);

            //player.wood
            jPlayersWood[i] = json_object_new_int(game.players[i].wood);
            json_object_object_add(jPlayers[i], "wood", jPlayersWood[i]);

            json_object_array_add(jPlayersArray, jPlayers[i]);
    }
    json_object_object_add(jGame, "players", jPlayersArray);

    //game.map
    json_object * jMap = json_object_new_object();

    //map.size
    json_object * jMapSize = json_object_new_int(game.map.size);
    json_object_object_add(jMap, "size", jMapSize);

    //map.nResources;
    json_object * jNResources = json_object_new_int(game.map.nResources);
    json_object_object_add(jMap, "nResources", jNResources);

    //map.resources;
    json_object ** jResources = (json_object**) malloc(game.map.nResources * sizeof(json_object*));
    json_object * jResourcesArray = json_object_new_array();
        json_object ** jResourcesType = (json_object**) malloc(game.map.nResources * sizeof(json_object*));
        json_object ** jResourcesPos = (json_object**) malloc(game.map.nResources * sizeof(json_object*));
            json_object ** jResourcesX = (json_object**) malloc(game.map.nResources * sizeof(json_object*));
            json_object ** jResourcesY = (json_object**) malloc(game.map.nResources * sizeof(json_object*));

    for(int i =0; i<game.map.nResources; i++){
        jResources[i] = json_object_new_object();

        //Resource.type
        jResourcesType[i] = json_object_new_int(game.map.resources[i].type);
        json_object_object_add(jResources[i], "type", jResourcesType[i]);

        //Resource.pos
        jResourcesPos[i] = json_object_new_object();

        jResourcesX[i] = json_object_new_int(game.map.resources[i].pos.x);
        json_object_object_add(jResourcesPos[i], "x", jResourcesX[i]);

        jResourcesY[i] = json_object_new_int(game.map.resources[i].pos.y);
        json_object_object_add(jResourcesPos[i], "y", jResourcesY[i]);

        json_object_object_add(jResources[i], "pos", jResourcesPos[i]);

        //Appending to array
        json_object_array_add(jResourcesArray, jResources[i]);
    }
    json_object_object_add(jMap, "resources", jResourcesArray);

    json_object_object_add(jGame, "map", jMap);

        //Copy of string to the JSON file
    char *  path = (char*) malloc(100 * sizeof(char));
    path[0] = '\0';
    path = strcat(path, "saves/");
    path = strcat(path, saveName);
    path = strcat(path, ".json");

    FILE * save = fopen(path, "w");

    fputs(json_object_to_json_string(jGame), save);

        //Freeing
    json_object_put(jGame);  //All children object are alsoo freed
    free(path);
    fclose(save);


    free(jPlayers);
    free(jPlayersId);
    free(jPlayersNBuildings),
    free(jPlayersNUnits);
    free(jPlayersBuildingsArray);
    free(jPlayersUnitsArray);
    free(jPlayersGold);
    free(jPlayersWood);

    for(int i=0; i<game.nPlayers; i++){
        free(jPlayersBuildings[i]);

            free(jBuildingsType[i]);
            free(jBuildingsOwner[i]);
            free(jBuildingsPos[i]);
                free(jBuildingsX[i]);
                free(jBuildingsY[i]);
            free(jBuildingsLife[i]);
            free(jBuildingsMaxLife[i]);
            free(jBuildingsIsBusy[i]);

            free(jPlayersUnits[i]);
                free(jUnitsType[i]);
                free(jUnitsOwner[i]);
                free(jUnitsPos[i]);
                    free(jUnitsX[i]);
                    free(jUnitsY[i]);
                free(jUnitsLife[i]);
                free(jUnitsMaxLife[i]);
                free(jUnitsAttack[i]);
                free(jUnitsIsBusy[i]);
                free(jUnitsMovements[i]);
                free(jUnitsMaxMovements[i]);
    }

    free(jPlayersBuildings);

        free(jBuildingsType);
        free(jBuildingsOwner);
        free(jBuildingsPos);
            free(jBuildingsX);
            free(jBuildingsY);
        free(jBuildingsLife);
        free(jBuildingsMaxLife);
        free(jBuildingsIsBusy);

    free(jPlayersUnits);
        free(jUnitsType);
        free(jUnitsOwner);
        free(jUnitsPos);
            free(jUnitsX);
            free(jUnitsY);
        free(jUnitsLife);
        free(jUnitsMaxLife);
        free(jUnitsAttack);
        free(jUnitsIsBusy);
        free(jUnitsMovements);
        free(jUnitsMaxMovements);

        free(jResources);
            free(jResourcesType);
            free(jResourcesPos);
                free(jResourcesX);
                free(jResourcesY);
}

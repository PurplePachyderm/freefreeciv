#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "../include/game/save_system.h"


void createSave(char * saveName, game game){

        //Creation of JSON string
    json_object * jsonGame = json_object_new_object();

    //game.nPLayers
    json_object * jsonNPlayers = json_object_new_int(game.nPlayers);
    json_object_object_add(jsonGame, "nPlayers", jsonNPlayers);

    //game.players
    json_object * jsonPlayers = json_object_new_array();

    json_object ** jsonPlayer = (json_object*) malloc(game.nPlayers * sizeof(json_object));
    for(int i=0; i<game.nPlayers; i++){
        jsonPlayer[i] = json_object_new_object();

        //player.id
        json_object * jsonId = json_object_new_int(game.players[i].id);
        json_object_object_add(jsonPlayer, "id", jsonId);

        //player.nBuildings
        json_object * jsonNBuildings = json_object_new_int(game.players[i].nBuildings);
        json_object_object_add(jsonPlayer, "nBuildings", jsonNBuildings);

        //players.buildings
        json_object * jsonBuildings = json_object_new_array();

        for(int j=0; j<game.players[i].nBuildings; j++){
            json_object * jsonBuilding = json_object_new_object();

            //building.type
            json_object * jsonType = json_object_new_int(game.players[i].buildings[j].type);
            json_object_object_add(jsonBuilding, "type", jsonType);

            //building.owner
            json_object * jsonOwner = json_object_new_int(game.players[i].buildings[j].owner);
            json_object_object_add(jsonBuilding, "owner", jsonOwner);

            //building.pos
            json_object * jsonPos = json_object_new_object();

            json_object * jsonX = json_object_new_int(game.players[i].buildings[j].pos.x);
            json_object_object_add(jsonPos, "x", jsonX);

            json_object * jsonY = json_object_new_int(game.players[i].buildings[j].pos.y);
            json_object_object_add(jsonPos, "y", jsonY);

            json_object_object_add(jsonBuilding, "pos", jsonPos);

            //building.life
            json_object * jsonLife = json_object_new_int(game.players[i].buildings[j].life);
            json_object_object_add(jsonBuilding, "life", jsonLife);

            //building.maxLife
            json_object * jsonMaxLife = json_object_new_int(game.players[i].buildings[j].maxLife);
            json_object_object_add(jsonBuilding, "maxLife", jsonMaxLife);

            //building.isBusy
            json_object * jsonIsBusy = json_object_new_int(game.players[i].buildings[j].isBusy);
            json_object_object_add(jsonBuilding, "isBusy", jsonIsBusy);


            //Append to array
            json_object_array_add(jsonBuildings, jsonBuilding);
        }

        json_object_object_add(jsonPlayer, "buildings", jsonBuildings);

        //player.nUnits
        json_object * jsonNUnits = json_object_new_int(game.players[i].nUnits);
        json_object_object_add(jsonPlayer, "nUnits", jsonNUnits);

        //player.units
        json_object * jsonUnits = json_object_new_array();

        for(int j=0; j<game.players[i].nUnits; j++){
            json_object * jsonUnit = json_object_new_object();

            //unit.type
            json_object * jsonType = json_object_new_int(game.players[i].units[j].type);
            json_object_object_add(jsonUnit, "type", jsonType);

            //unit.owner
            json_object * jsonOwner = json_object_new_int(game.players[i].units[j].owner);
            json_object_object_add(jsonUnit, "owner", jsonOwner);

            //unit.pos
            json_object * jsonPos = json_object_new_object();

            json_object * jsonX = json_object_new_int(game.players[i].units[j].pos.x);
            json_object_object_add(jsonPos, "x", jsonX);

            json_object * jsonY = json_object_new_int(game.players[i].units[j].pos.y);
            json_object_object_add(jsonPos, "y", jsonY);

            json_object_object_add(jsonUnit, "pos", jsonPos);

            //unit.life
            json_object * jsonLife = json_object_new_int(game.players[i].units[j].life);
            json_object_object_add(jsonUnit, "life", jsonLife);

            //unit.maxLife
            json_object * jsonMaxLife = json_object_new_int(game.players[i].units[j].maxLife);
            json_object_object_add(jsonUnit, "maxLife", jsonMaxLife);

            //unit.attack
            json_object * jsonAttack = json_object_new_int(game.players[i].units[j].attack);
            json_object_object_add(jsonUnit, "attack", jsonAttack);

            //unit.isBusy
            json_object * jsonIsBusy = json_object_new_int(game.players[i].units[j].isBusy);
            json_object_object_add(jsonUnit, "isBusy", jsonIsBusy);

            //unit.movements
            json_object * jsonMovements = json_object_new_int(game.players[i].units[j].movements);
            json_object_object_add(jsonUnit, "movements", jsonMovements);

            //unit.maxMovement
            json_object * jsonMaxMovements = json_object_new_int(game.players[i].units[j].maxMovements);
            json_object_object_add(jsonUnit, "maxMovement", jsonMaxMovements);

            //Append to array
            json_object_array_add(jsonUnits, jsonUnit);
        }

        json_object_object_add(jsonPlayer, "players", jsonUnits);

        //player.gold
        json_object * jsonGold = json_object_new_int(game.players[i].gold);
        json_object_object_add(jsonPlayer, "gold", jsonGold);

        //player.wood
        json_object * jsonWood = json_object_new_int(game.players[i].wood);
        json_object_object_add(jsonPlayer, "wood", jsonWood);

        //Append to array
        json_object_array_add(jsonPlayers, jsonPlayer);
    }

    json_object_object_add(jsonGame, "players", jsonPlayers);

    //game.map
    json_object * jsonMap = json_object_new_object();

    //map.size
    json_object * jsonSize = json_object_new_int(game.map.size);
    json_object_object_add(jsonMap, "size", jsonSize);

    //map.nResources
    json_object * jsonNResources = json_object_new_int(game.map.nResources);
    json_object_object_add(jsonMap, "nResources", jsonNResources);

    //map.resources
    json_object * jsonResources = json_object_new_array();

    for(int i=0; i<game.map.nResources; i++){
        json_object * jsonResource = json_object_new_object();

        //resource.type
        json_object * jsonType = json_object_new_int(game.map.resources[i].type);
        json_object_object_add(jsonMap, "nResources", jsonType);

        //resource.pos
        json_object * jsonPos = json_object_new_object();

        json_object * jsonX = json_object_new_int(game.map.resources[i].pos.x);
        json_object_object_add(jsonPos, "x", jsonX);

        json_object * jsonY = json_object_new_int(game.map.resources[i].pos.y);
        json_object_object_add(jsonPos, "y", jsonY);

        json_object_object_add(jsonResource, "pos", jsonPos);

        //Append to array
        json_object_array_add(jsonResources, jsonResource);
    }

    json_object_object_add(jsonMap, "resources", jsonResources);

    json_object_object_add(jsonGame, "map", jsonMap);

//BUG This code will cause a segfault because every pointer needs to stay defined if it is included in an object

        //Copy of string to the JSON file
    char *  path = (char*) malloc(100 * sizeof(char));
    path = strcat(path, "saves/");
    char * fileName = strcat(saveName, ".json");
    path = strcat(path, saveName);

    printf("%s", path);

    FILE * save = fopen(saveName, "w");

    fputs(json_object_to_json_string(jsonGame), save);

    json_object_put(jsonGame);
    free(path);
    free(fileName);
    fclose(save);
}

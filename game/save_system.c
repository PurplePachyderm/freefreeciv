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
    free(jsonNPlayers);

    //game.players
    json_object * jsonPlayers = json_object_new_array();

    for(int i=0; i++; i<game.nPlayers){
        json_object * jsonPlayer = json_object_new_object();

        //player.id
        json_object * jsonId = json_object_new_int(game.players[i].id);
        json_object_object_add(jsonPlayer, "id", jsonId);
        free(jsonId);

        //player.nBuildings
        json_object * jsonNBuildings = json_object_new_int(game.players[i].nBuildings);
        json_object_object_add(jsonPlayer, "nBuildings", jsonNBuildings);
        free(jsonNBuildings);

        //players.buildings
        json_object * jsonBuildings = json_object_new_array();

        for(int j=0; j++; j<game.players[0].nBuildings){
            json_object * jsonBuilding = json_object_new_object();

            //building.type
            json_object * jsonType = json_object_new_int(game.players[i].buildings[j].type);
            json_object_object_add(jsonBuilding, "type", jsonType);
            free(jsonType);
        }

    }


        //Copy of string to the JSON file
    char *  path = (char*) malloc(100 * sizeof(char));
    path = strcat(path, saveName);
    path = strcat(path, ".json");

    //FILE * save = fopen(saveName, "w");

}

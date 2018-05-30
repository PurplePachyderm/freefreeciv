#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "../include/multiplayer/json.h"



int parseRooms(room ** rooms, const char * jString){
    //Returns the number of rooms and allocates the rooms pointer

    int nRooms;

    json_object * json = json_tokener_parse(jString);

    //Get number of rooms
    json_object * jNRooms = json_object_object_get(json, "nRooms");

    if(jNRooms == NULL){
        nRooms = -1;
    }

    else{   //If conversion has been successful
        nRooms = json_object_get_int(jNRooms);
        free(jNRooms);

        *rooms = (room*) malloc(nRooms * sizeof(room));

        json_object * jRooms = json_object_object_get(json, "rooms");

        for(int i=0; i<nRooms; i++){
            json_object * jRoom = json_object_array_get_idx(jRooms, i);

            //Get name
            (*rooms)[i].name = (char*) malloc(100*sizeof(char));
            json_object * jName = json_object_object_get(jRoom, "name");
            sprintf((*rooms)[i].name, json_object_get_string(jName));
            free(jName);

            //Get nPlayers
            json_object * jNPlayers = json_object_object_get(jRoom, "nPlayers");
            (*rooms)[i].nPlayers = json_object_get_int(jNPlayers);
            free(jNPlayers);

            //Get host
            (*rooms)[i].host = (char*) malloc(100*sizeof(char));
            json_object * jHost = json_object_object_get(jRoom, "host");
            sprintf((*rooms)[i].host, json_object_get_string(jHost));
            free(jHost);

            free(jRoom);
        }

        free(jRooms);

    }

    return nRooms;
}



int parsePlayers(mPlayer ** players, const char * jString){
    //Retrieves all the players and ther infos in a room

    json_object * json = json_tokener_parse(jString);

    //Get number of players
    json_object * jNPlayers = json_object_object_get(json, "nPlayers");
    int nPlayers = json_object_get_int(jNPlayers);
    free(jNPlayers);

    *players = (mPlayer*) malloc(nPlayers * sizeof(mPlayer));

    json_object * jPlayers = json_object_object_get(json, "players");

    for(int i=0; i<nPlayers; i++){
        json_object * jPlayer = json_object_array_get_idx(jPlayers, i);


        //Get pseudo
        (*players)[i].pseudo = (char*) malloc(100*sizeof(char));
        json_object * jPseudo = json_object_object_get(jPlayer, "pseudo");
        sprintf((*players)[i].pseudo, json_object_get_string(jPseudo));
        free(jPseudo);

        //Get isAIControlled
        json_object * jIsAIControlled = json_object_object_get(jPlayer, "isAIControlled");
        (*players)[i].isAIControlled = json_object_get_int(jIsAIControlled);
        free(jIsAIControlled);

        //Get slot
        json_object * jSlot = json_object_object_get(jPlayer, "slot");
        (*players)[i].slot = json_object_get_int(jSlot);
        free(jSlot);


        free(jPlayer);
    }

    free(jPlayers);

    return nPlayers;
}



char * serializePlayerSelf(char * pseudo, int nPlayers, int roomId){
    //Allows  to create a player JSON string for yourself

    char * jsonString = (char *) malloc(400 * sizeof(char));

    json_object * json = json_object_new_object();

    json_object * jEventType = json_object_new_int(PLAYER_JOIN_ROOM);
    json_object * jRoomId = json_object_new_int(roomId);
    json_object * jPseudo = json_object_new_string(pseudo);
    json_object * jIsAIControlled = json_object_new_int(PLAYER);
    json_object * jSlot = json_object_new_int(nPlayers);

    json_object_object_add(json, "eventType", jEventType);
    json_object_object_add(json, "roomId", jRoomId);
    json_object_object_add(json, "pseudo", jPseudo);
    json_object_object_add(json, "isAIControlled", jIsAIControlled);
    json_object_object_add(json, "slot", jSlot);

    sprintf(jsonString, json_object_to_json_string(json));

    json_object_put(json);

    return jsonString;
}



mEvent parseEvent(const char * jString){
    //WARNING Start game event not handled (since it needs the entire game object as parameter, too complicated)

    json_object * json = json_tokener_parse(jString);
    mEvent event;

    //Most of the values won't be used (and not received) for one type of event

    //Get type
    json_object * jType = json_object_object_get(json, "type");
    event.type = json_object_get_int(jType);
    free(jType);

    //Get roomId
    json_object * jRoomId = json_object_object_get(json, "roomId");
    event.roomId = json_object_get_int(jRoomId);
    free(jRoomId);

    //Get clientId
    json_object * jClientId = json_object_object_get(json, "clientId");
    event.clientId = json_object_get_int(jClientId);
    free(jClientId);

    //Get playerInfos
    json_object * jPlayerInfos = json_object_object_get(json, "playerInfos");

        //Get pseudo
    event.playerInfos.pseudo = (char*) malloc(100*sizeof(char));
    json_object * jPseudo = json_object_object_get(jPlayerInfos, "pseudo");
    sprintf(event.playerInfos.pseudo, json_object_get_string(jPseudo));
    free(jPseudo);

        //Get isAIControlled
    json_object * jIsAIControlled = json_object_object_get(jPlayerInfos, "isAIControlled");
    event.playerInfos.isAIControlled = json_object_get_int(jIsAIControlled);
    free(jIsAIControlled);

        //Get slot
    json_object * jSlot = json_object_object_get(jPlayerInfos, "slot");
    event.playerInfos.slot = json_object_get_int(jSlot);
    free(jSlot);

    free(jPlayerInfos);


    //Get unitId
    json_object * jUnitId = json_object_object_get(json, "unitId");
    event.unitId = json_object_get_int(jUnitId);
    free(jUnitId);

    //Get coord.x
    json_object * jCoordX = json_object_object_get(json, "coordX");
    event.target.x = json_object_get_int(jCoordX);
    free(jCoordX);

    //Get coord.y
    json_object * jCoordY = json_object_object_get(json, "coordY");
    event.target.y = json_object_get_int(jCoordY);
    free(jCoordY);

    return event;
}



char * serializeEvent(mEvent event){
    printf("Entering serializeEvent\n");
    char * jsonString = (char *) malloc(3000 * sizeof(char));

    json_object * json = json_object_new_object();

    json_object * jType = json_object_new_int(event.type);
    json_object * jRoomId = json_object_new_int(event.roomId);
    json_object * jClientId = json_object_new_int(event.clientId);
    json_object * jPlayerInfos = json_object_new_object();
        json_object * jPseudo = json_object_new_string(event.playerInfos.pseudo);
        json_object * jIsAIControlled = json_object_new_int(event.playerInfos.isAIControlled);
        json_object * jSlot = json_object_new_int(event.playerInfos.slot);
    json_object * jUnitId = json_object_new_int(event.unitId);
    json_object * jCoordX = json_object_new_int(event.target.x);
    json_object * jCoordY = json_object_new_int(event.target.y);

    json_object_object_add(json, "type", jType);
    json_object_object_add(json, "roomId", jRoomId);
    json_object_object_add(json, "clientId", jClientId);
    json_object_object_add(json, "playerInfos", jPlayerInfos);
        json_object_object_add(jPlayerInfos, "pseudo", jPseudo);
        json_object_object_add(jPlayerInfos, "isAIControlled", jIsAIControlled);
        json_object_object_add(jPlayerInfos, "slot", jSlot);
    json_object_object_add(json, "unitId", jUnitId);
    json_object_object_add(json, "coordX", jCoordX);
    json_object_object_add(json, "coordY", jCoordY);


    sprintf(jsonString, json_object_to_json_string(json));

    json_object_put(json);

    return jsonString;
}

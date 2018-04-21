#include <stdio.h>
#include <stdlib.h>

#include "../include/multiplayer/game_init.hpp"
#include "../include/multiplayer/easywsclient.hpp"
#include "../include/multiplayer/json.h"
#include "../include/display/display.h"
#include <json-c/json.h>

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <string>


int wsConnect(SDL_Renderer * renderer, SDL_Texture * texture){
    //Initializes connexion with the server and calls the lobby
    printf("wsConnect\n");

    int exitCode = 0;

    using easywsclient::WebSocket;
    static WebSocket::pointer ws = NULL;

     #ifdef _WIN32
         INT rc;
         WSADATA wsaData;

         rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
         if (rc) {
             printf("WSAStartup Failed.\n");
             return 1;
         }
     #endif

    //ws = WebSocket::from_url("ws://192.168.0.189:8080");
    ws = WebSocket::from_url("ws://127.0.0.1:8080");
    assert(ws);

    //Send pseudo
    char * jsonString = (char *) malloc(100 * sizeof(char));
    json_object * json = json_object_new_object();

    json_object * jEventType = json_object_new_int(CONNECTION);
    json_object * jPseudo = json_object_new_string("Jacques");  //Hardcoded Pseudo for testing

    json_object_object_add(json, "eventType", jEventType);
    json_object_object_add(json, "pseudo", jPseudo);

    sprintf(jsonString, json_object_to_json_string(json));
    json_object_put(json);

    ws->send(jsonString);
    free(jsonString);


    //Lobby
    lobby(ws, renderer, texture);

    delete ws;
    #ifdef _WIN32
         WSACleanup();
    #endif
    return exitCode;
}



     //Lobby
class lobbyIntermediary {
public:
    room * rooms;
    int nRooms;
    int roomToJoin;
    char * pseudo;

    void callbackLobby(const std::string & message, lobbyIntermediary * instance){
        printf("Receiving: %s", &message[0]);

        //Checks if room list is sent
        int newNRooms = parseRooms(&(instance->rooms), &message[0]);

        //If we received a room list, the number of rooms takes the new value
        if(newNRooms != -1){
            instance->nRooms = newNRooms;

            for(int i=0; i<instance->nRooms; i++){
                if(strcmp(instance->pseudo, instance->rooms[i].host) == 0){
                    instance->roomToJoin = i;
                    break;
                }
            }
        }


        else{   //Update in the existing rooms
            event event = parseEvent(&message[0]);

            if(event.type == PLAYER_JOIN_ROOM){
                (instance->rooms)[event.roomId].nPlayers++;
            }
            else if(event.type == PLAYER_LEAVE_ROOM){
                (instance->rooms)[event.roomId].nPlayers--;
            }
        }

        printf("\n");

    }
};


class lobbyFunctor {
  public:
      lobbyIntermediary * instance;
      int nRooms;

    lobbyFunctor(lobbyIntermediary * instance) : instance(instance) {
    }
    void operator()(const std::string& message) {
        instance->callbackLobby(message, instance);
    }
};



int lobby(easywsclient::WebSocket * ws, SDL_Renderer * renderer, SDL_Texture * texture){
    //Menu wwith all the current rooms avalaible
    //Allows to join one of them (creation with the companion app)
    int quit = 0;

    SDL_Event eventSDL;

    //Stores data that will be modified by server inputs
    lobbyIntermediary instance;
    instance.rooms = NULL;
    instance.nRooms = 0;
    instance.roomToJoin = -1;
    instance.pseudo = (char*) malloc(100*sizeof(char));
    sprintf(instance.pseudo, "Jacques");    //Hardcode pseudo for now

    lobbyFunctor functor(&instance);

    //Rooms request
    char * request = (char *)malloc(13*sizeof(char));
    sprintf(request, "roomsRequest");
    ws->send(request);
    free(request);

    //Sets up callback function
    while(ws->getReadyState() != easywsclient::WebSocket::CLOSED && !quit){
        ws->poll();
        SDL_Delay(50);
        ws->dispatch(functor);

        //If in lobby
        if(instance.roomToJoin == -1){
            system("clear");
            for(int i=0; i<instance.nRooms; i++){
                printf("roomName: %s   nPlayers: %d   host:%s\n", instance.rooms[i].name, instance.rooms[i].nPlayers, instance.rooms[i].host);
            }


            while(SDL_PollEvent(&eventSDL)){
                if(eventSDL.type == SDL_KEYDOWN){
                    switch(eventSDL.key.keysym.sym){
                        case(SDLK_q):
                            quit = 1;
                            break;

                        case(SDLK_KP_0):
                            instance.roomToJoin = 0;
                            break;

                        case(SDLK_KP_1):
                            instance.roomToJoin = 1;
                            break;
                    }
                }
            }
        }

        //Sending request via roomFunction
        else{
            roomFunction(ws, renderer, texture, instance.rooms[instance.roomToJoin], instance.roomToJoin);
        }

    }

    return 0;
}



    //Room
class roomIntermediary {
public:
    mPlayer * players;
    int nPlayers;
    int readyToPlay;

    void callbackRoom(const std::string & message, mPlayer ** players, int * nPlayers, int *readyToPlay){
        getchar();

        //Checking if game is starting
        json_object * json = json_tokener_parse(&message[0]);
        json_object * jType = json_object_object_get(json, "type");

        //If receiving a game start event
        if(jType != NULL){
            int type = json_object_get_int(jType);
            free(jType);

            if(type == GAME_START){
                *readyToPlay = 1;
            }
        }

        //Else, update players list
        else
            *nPlayers = parsePlayers(players, &message[0]);

    }
};


class roomFunctor {
  public:
      roomIntermediary * instance;

    roomFunctor(roomIntermediary * instance) : instance(instance) {
    }
    void operator()(const std::string& message) {
        instance->callbackRoom(message, &(instance->players), &(instance->nPlayers), &(instance->readyToPlay));
    }
};



int roomFunction(easywsclient::WebSocket * ws, SDL_Renderer * renderer, SDL_Texture * texture, room room, int roomId){
    //Menu wwith all the current rooms avalaible
    //Allows to join one of them (creation with the companion app)
    int quit = 0;

    //Sending request of players and own infos
    mPlayer player;
    player.pseudo = (char*) malloc(100*sizeof(char));
    sprintf(player.pseudo, "Jean-Charles Babos IV");    //Get own pseudo in external file
    player.isAIControlled = 0;
    coord target;
    target.x = -1;
    target.y = -1;
    event eventJoinRoom = {PLAYER_JOIN_ROOM, roomId, -1, player, -1, target};
    // eventJoinRoom.type = PLAYER_JOIN_ROOMY;
    // eventJoinRoom.roomId = roomId;
    // eventJoinRoom.playerInfos = player;


    char * jString;
    jString = serializeEvent(eventJoinRoom);
    ws->send(jString);
    free(jString);


    roomIntermediary instance;
    instance.players = NULL;
    instance.nPlayers = -1;
    instance.readyToPlay = 0;

    roomFunctor functor(&instance);

    while(ws->getReadyState() != easywsclient::WebSocket::CLOSED && !quit){
        ws->poll();
        SDL_Delay(50);
        ws->dispatch(functor);
        system("clear");

        printf("nPlayers: %d\n", instance.nPlayers);
        for(int i=0; i<instance.nPlayers; i++){
            printf("playerName: %s   isAIControlled: %d\n", instance.players[i].pseudo, instance.players[i].isAIControlled);
        }

        if(instance.readyToPlay){
            //inGame(ws, ...);
            printf("Yeah =)\n");
        }

    }

    return 0;
}

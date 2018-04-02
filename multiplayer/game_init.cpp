#include <stdio.h>
#include <stdlib.h>

#include "../include/multiplayer/game_init.hpp"
#include "../include/multiplayer/easywsclient.hpp"
#include "../include/multiplayer/json.h"

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <string>

    //Connexion
//For testing purpose only
int wsConnectTest(char * string){
    //int exitCode;

    using easywsclient::WebSocket;
    WebSocket * ws = NULL;

     #ifdef _WIN32
         INT rc;
         WSADATA wsaData;

         rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
         if (rc) {
             printf("WSAStartup Failed.\n");
             return 1;
         }
     #endif

    ws = WebSocket::from_url("ws://192.168.0.189:8080");
    assert(ws);
    ws->send(string);
    while (ws->getReadyState() != WebSocket::CLOSED) {
      ws->poll();
    }
    delete ws;
    #ifdef _WIN32
         WSACleanup();
    #endif
    return 0;
}



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

    ws = WebSocket::from_url("ws://192.168.0.189:8080");
    assert(ws);

    //Lobby
    lobby(ws, renderer, texture);

    delete ws;
    #ifdef _WIN32
         WSACleanup();
    #endif
    return exitCode;
}



//     //Lobby
// class lobbyIntermediary{
//
//     room ** rooms;
//
//     void callbackLobby(const std::string & message){
//
//         //Checks if room list is sent
//         parseRooms(this.rooms, &message[0]);
//         if(*(this.rooms) != NULL){ //Parsing worked, event provided the whole rooms list
//             returnValue = 1;
//         }
//         else{
//             //Checks if update is sent
//             //TODO Parse and treat room update
//             printf("Not rooms list\n");
//         }
//     }
// };
//
//
//
// class Functor {
//     lobbyIntermediary * lobbyintermediary;
//   public:
//     Functor(lobbyIntermediary * lobbyintermediary) : lobbyIntermediary(lobbyIntermediary) {
//     }
//     void operator()(const std::string& message) {
//         lobbyintermediary->callbackLobby(message);
//     }
// };
class lobbyIntermediary {
private:
public:
    room ** rooms;
  void callbackLobby(const std::string & message){
        printf("Callback: %s\n", &message[0]);
        //Checks if room list is sent
        //*rooms = NULL;
        parseRooms(rooms, &message[0]);

        if(*(rooms) == NULL){ //Parsing worked, event provided the whole rooms list
            //Checks if update is sent
            //TODO Parse and treat room update
            printf("Not rooms list\n");
        }
    }
};


class Functor {
  public:
      lobbyIntermediary * instance;

    Functor(lobbyIntermediary * instance) : instance(instance) {
    }
    void operator()(const std::string& message) {
        instance->callbackLobby(message);
    }
};



int lobby(easywsclient::WebSocket * ws, SDL_Renderer * renderer, SDL_Texture * texture){
    printf("lobby\n");

    room * rooms;
    lobbyIntermediary instance;
    instance.rooms = &rooms;
    Functor functor(&instance);

    //Rooms request
    char * jString = (char *)malloc(300*sizeof(char));
    sprintf(jString, roomRequest());
    ws->send(jString);
    free(jString);


    //Sets up callback function
    while(ws->getReadyState() != easywsclient::WebSocket::CLOSED){
        ws->poll();
        ws->dispatch(functor);
    }

    return 0;
}

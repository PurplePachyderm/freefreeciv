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

int roomFunction(easywsclient::WebSocket * ws, SDL_Renderer * renderer, SDL_Texture * texture, room room, int roomId){

    return 0;
}

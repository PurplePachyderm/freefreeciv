#include <stdio.h>
#include <stdlib.h>

#include "../include/multiplayer/game_init.hpp"
#include "../include/multiplayer/easywsclient.hpp"

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <string>

using easywsclient::WebSocket;
static WebSocket::pointer ws = NULL;

int wsConnect(){
    //int exitCode;

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
    ws->send("ping");
    while (ws->getReadyState() != WebSocket::CLOSED) {
      ws->poll();
    }
    delete ws;
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}

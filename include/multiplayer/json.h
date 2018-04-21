#pragma once
#include "game_init.hpp"


int parseRooms(room ** rooms, const char * jString);

int parsePlayers(mPlayer ** players, const char * jString);
char * serializePlayerSelf(char * pseudo, int nPlayers, int roomId);

event parseEvent(const char * jString);
char * serializeEvent(event event);

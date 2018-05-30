#pragma once
#include "game_init.hpp"


int parseRooms(room ** rooms, const char * jString);

int parsePlayers(mPlayer ** players, const char * jString);
char * serializePlayerSelf(char * pseudo, int nPlayers, int roomId);

mEvent parseEvent(const char * jString);
char * serializeEvent(mEvent event);

#include "game.h"

#define MIN_DIST 7
#define N_RESOURCES 5   //Number for each type

void genSpawns(game * game);
void genResources(game * game);
void genGame(game * game, int nPlayers, int * isAIControlled);

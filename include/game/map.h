#include "game.h"

#define MIN_DIST 7
#define N_RESOURCES 5   //Number for each type

void genSpawns(struct game * game);
void genResources(struct game * game);
void genGame(struct game * game, int nPlayers, int * isAIControlled);

#include "../coord.h"
#include "game.h"

int checkMap(game game, coord pos);
void pathTesting(game game, coord startPos, coord targetPos, int size, coord * path, int currentIndex, int * pathFound);
int moveUnit(game * game, int ownerId, int unitId, coord targetPos, coord ** path);

void collect(game * game, int ownerId, int unitId, coord targetPos);

void attack(game * game, int ownerId, int unitId, coord targetPos);

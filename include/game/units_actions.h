#include "../coord.h"
#include "game.h"

int checkMap(struct game game, coord pos);
int pathTesting(struct game game, coord startPos, coord targetPos, int size, coord * path, int currentIndex, int * pathFound, int * nIterations);
int moveUnit(struct game * game, int unitId, coord targetPos, coord ** path);

void collect(struct game * game, int unitId, coord targetPos);

void attack(struct game * game, int unitId, coord targetPos);

void createPeasant(struct game * game, coord pos, int cityId);
void createSoldier(struct game * game, coord pos, int barrackId);
void createBarrack(struct game * game, coord pos, int peasantId);

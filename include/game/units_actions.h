#include "../coord.h"
#include "game.h"

int checkMap(struct game game, coord pos);
int pathTesting(struct game game, coord startPos, coord targetPos, int size, coord * path, int currentIndex, int * pathFound, int * nIterations);
int moveUnit(struct game * game, int unitId, coord targetPos, coord ** path);

int collect(struct game * game, int unitId, coord targetPos);

int attack(struct game * game, int unitId, coord targetPos);

int createPeasant(struct game * game, coord pos, int cityId);
int createSoldier(struct game * game, coord pos, int barrackId);
int createBarrack(struct game * game, coord pos, int peasantId);

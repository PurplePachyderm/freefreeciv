#include "../coord.h"
#include "game.h"

int checkMap(struct struct game game, coord pos);
void pathTesting(struct struct game game, coord startPos, coord targetPos, int size, coord * path, int currentIndex, int * pathFound);
int moveUnit(struct struct game * game, int unitId, coord targetPos, coord ** path);

void collect(struct struct game * game, int unitId, coord targetPos);

void attack(struct struct game * game, int unitId, coord targetPos);

void createPeasant(struct struct game * game, coord pos, int cityId);
void createSoldier(struct struct game * game, coord pos, int barrackId);
void createBarrack(struct struct game * game, coord pos, int peasantId);

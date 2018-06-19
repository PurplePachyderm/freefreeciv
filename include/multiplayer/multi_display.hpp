#include "../display/display.h"
#include "../game/game.h"
#include <json-c/json.h>

void mBasicDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int isMainHud, char * pseudo);
void mMainDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, char * pseudo);
void mPeasantDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int peasantId, char * pseudo);
void mSoldierDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int soldierId, char * pseudo);
void mBuildingDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int buildingId, char * pseudo);
void mTargetDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int isMovement, coord pos, char * pseudo);
void mForeignDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int ownerId, int tokenId, int isUnit, char * pseudo);

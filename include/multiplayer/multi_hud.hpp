//Contains display of different Huds in game (called in Hud functions)
//Multiplayer equivalent to "include/display/hud_display.c"

#include "../display/display.h"
#include "../game/game.h"

void mBasicDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int isMainHud);
void mMainDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown);
void mPeasantDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int peasantId);
void mSoldierDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int soldierId);
void mBuildingDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int soldierId);
void mTargetDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int isMovement, coord pos);
void mForeignDisplay(SDL_Renderer * renderer, SDL_Texture * texture, struct game game, view camera, int countdown, int ownerId, int tokenId, int isUnit);

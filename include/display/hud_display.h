//Contains display of different Huds in game (called in Hud functions)
#include "display.h"
#include "../game/game.h"

void basicDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int isMainHud);
void mainDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown);
void peasantDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int peasantId);
void soldierDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
void buildingDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
void targetDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int isMovement, coord pos);
void foreignDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int ownerId, int tokenId, int isUnit);

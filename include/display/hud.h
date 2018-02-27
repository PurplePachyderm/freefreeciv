//Contains game "loops" functions (one for each hud to be displayed)
#include "display.h"
#include "../game/game.h"


int countdownUpdate(int * countdown, int * countdownSec, int quit, int * newEvent, game * game);

void mainHud(SDL_Renderer * renderer, SDL_Texture * texture, game game);
int peasantHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int peasantId);   //This is way too long
int soldierHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int soldierId);
int buildingHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int soldierId);
int targetHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int isMovement, coord pos, coord * target);
int foreignHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int ownerId, int tokenId, int isUnit);

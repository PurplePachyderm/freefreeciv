//Contains game "loops" functions (one for each hud to be displayed)
#include "display.h"
#include "../game/game.h"

SDL_Rect basicDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, char * message);
int countdownUpdate(int * countdown, int * countdownSec, int quit, int * newEvent, game * game);

SDL_Rect mainDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown);
void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game);

SDL_Rect peasantDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, int peasantId);
int peasantHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int peasantId);   //This is way too long

SDL_Rect soldierDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
int soldierHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int soldierId);

SDL_Rect buildingDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
int buildingHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int soldierId);

SDL_Rect targetDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, int isMovement, coord pos);
int targetHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int isMovement, coord pos, coord * target);

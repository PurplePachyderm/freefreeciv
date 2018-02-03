//Contains game "loops" functions (one for each hud to be displayed)
#include "display.h"
#include "../game/game.h"

SDL_Rect mainHudDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown);
void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game);

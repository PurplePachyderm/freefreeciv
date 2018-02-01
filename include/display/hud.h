//Contains game "loops" functions (one for each hud to be displayed)
#include "display.h"
#include "../game/game.h"

void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game);
int menuHud(SDL_Renderer * renderer, SDL_Surface * sprites,  SDL_Texture * texture, game game);

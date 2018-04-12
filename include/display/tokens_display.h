#include "../coord.h"
#include "../game/game.h"
#include "display.h"

void dispUnits(SDL_Renderer * renderer, SDL_Texture * texture, view camera, player player);
void dispBuildings(SDL_Renderer * renderer, SDL_Texture * texture, view camera, player player);

void dispResources(SDL_Renderer * renderer, SDL_Texture * texture, view camera, struct struct map map);

void dispTokens(SDL_Renderer * renderer, SDL_Texture * texture, view camera, struct struct game game);

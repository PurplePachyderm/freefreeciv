#include "../coord.h"
#include "../game/game.h"
#include "display.h"

void dispUnits(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera, player player);
void dispBuildings(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera, player player);

void dispResources(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera, map map);

void dispTokens(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera, game game);

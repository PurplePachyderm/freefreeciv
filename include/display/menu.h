#include "../coord.h"
#include "../game/game.h"

void aff_carre(SDL_Renderer* RENDERER, int x, int y, int w, int h);
void change_couleur(SDL_Renderer* RENDERER);
void menu();

int inGameMenu(SDL_Renderer * renderer, SDL_Surface * sprites,  SDL_Texture * texture);

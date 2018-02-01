#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include/display/menu.h"
#include "include/display/hud.h"
#include "include/game/game.h"
#include "include/game/structures_init.h"
#include "include/game/save_system.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

		//Camera init


		//SDL Initialization
	SDL_Window  * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_FULLSCREEN,
            					&window,
            					&renderer);



	int flags = IMG_INIT_PNG;
	IMG_Init(flags);
	SDL_Surface * sprites = NULL;
	sprites = IMG_Load("resources/sprites.png");

	SDL_Texture * texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, sprites);

	TTF_Init();


		//Game init
	game game;
	loadSave("test", &game);

		//Display
	mainHud(renderer, sprites, texture, game);

	return 0;
}

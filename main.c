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
#include "include/game/map.h"
#include "include/multiplayer/game_init.hpp"
#include "include/multiplayer/json.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

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
	SDL_FreeSurface(sprites);

	TTF_Init();

		//Game init
	struct game game;
	//loadSave("test", &game);
	int nPlayers = 4;
	int * isAIControlled = (int*) malloc (4*sizeof(int));
	//Switch one of tose to 0 to add players
	isAIControlled[0] = 1;
	isAIControlled[1] = 1;
	isAIControlled[2] = 1;
	isAIControlled[3] = 1;
	genGame(&game, nPlayers, isAIControlled);

		//Main menu (game hud for now)
	mainHud(renderer, texture, game);


	//wsConnectTest(jString);
	//wsConnect(renderer, texture);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	TTF_Quit();
	IMG_Quit();

	return 0;
}

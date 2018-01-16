#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/display/map_display.h"
#include "include/display/tokens_display.h"
#include "include/display/menu.h"
#include "include/game/game.h"
#include "include/game/structures_init.h"
#include "include/game/save_system.h"
#include "include/game/units_actions.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

		//Camera init
	view camera;
	camera.offset.x = (SCREEN_WIDTH - (MAP_SIZE+2)*TILE_SIZE) / 2;	//Centers map
	camera.offset.y = (SCREEN_HEIGHT - (MAP_SIZE+2)*TILE_SIZE) / 2;
	camera.zoom = 1;
	camera.leftClick = 0;

		//SDL Initialization
	SDL_Window  * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_FULLSCREEN,
            					&window,
            					&renderer);

	SDL_Event event;

	int flags = IMG_INIT_PNG;
	IMG_Init(flags);
	SDL_Surface * sprites;
	sprites = IMG_Load("resources/sprites.png");

	SDL_Texture * texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, sprites);



		//Game init
	game game;
	loadSave("test", &game);

	coord targetPos;
	targetPos.x = 3;
	targetPos.y = 6;
	coord * path = NULL;
	int pathFound = moveUnit(&game, 0, 0, targetPos, &path);
	if(pathFound){
		printf("Path of %d tiles has been found!\n", pathFound);
		for(int i=0; i<pathFound; i++)
			printf("[%d,%d] ", path[i].x, path[i].y);
		printf("\n");
	}

		//Display
	//menu();
	int quit = 0;
	int newEvent;	//Render new frame only if a new event has occured to save resources

	dispMap(window, renderer, sprites, texture, camera); //Would'nt be displayed at first because of newEvent
	dispTokens(window, renderer, sprites, texture, camera, game);
	SDL_RenderPresent(renderer);
	while(!quit){
		SDL_Delay(REFRESH_PERIOD);
		newEvent = cameraEvents(&event, &camera);
		if(newEvent){
			dispMap(window, renderer, sprites, texture, camera);
			dispTokens(window, renderer, sprites, texture, camera, game);
			SDL_RenderPresent(renderer);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(sprites);

	return 0;
}

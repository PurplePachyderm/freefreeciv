#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/display/map_display.h"
#include "include/display/tokens_display.h"
#include "include/game/game.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

		//Camera init
	view camera;
	camera.offset.x = (SCREEN_WIDTH - MAP_SIZE*TILE_SIZE) / 2;	//Centers map
	camera.offset.y = (SCREEN_HEIGHT - MAP_SIZE*TILE_SIZE) / 2;
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
	sprites = IMG_Load("sprites.png");

	SDL_Texture * texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, sprites);


		//Game init
	game game;

	game.players = (player*) malloc(2 * sizeof(player));
	game.nPlayers = 2;

	game.players[0].id = RED;

	game.players[0].buildings = (building*) malloc(2 * sizeof(building));
	game.players[0].nBuildings = 2;
		game.players[0].buildings[0].type = CITY;
		game.players[0].buildings[0].pos.x = 3;
		game.players[0].buildings[0].pos.y = 5;

		game.players[0].buildings[1].type = BARRACK;
		game.players[0].buildings[1].pos.x = 5;
		game.players[0].buildings[1].pos.y = 2;


	game.players[0].units = (unit*) malloc(2 * sizeof(unit));
	game.players[0].nUnits = 2;
		game.players[0].units[0].type = PEASANT;
		game.players[0].units[0].pos.x = 3;
		game.players[0].units[0].pos.y = 4;

		game.players[0].units[1].type = SOLDIER;
		game.players[0].units[1].pos.x = 7;
		game.players[0].units[1].pos.y = 2;



	game.players[1].id = BLUE;

	game.players[1].buildings = (building*) malloc(2 * sizeof(building));
	game.players[1].nBuildings = 2;
		game.players[1].buildings[0].type = CITY;
		game.players[1].buildings[0].pos.x = 10;
		game.players[1].buildings[0].pos.y = 8;

		game.players[1].buildings[1].type = BARRACK;
		game.players[1].buildings[1].pos.x = 11;
		game.players[1].buildings[1].pos.y = 7;


	game.players[1].units = (unit*) malloc(2 * sizeof(unit));
	game.players[1].nUnits = 2;
		game.players[1].units[0].type = PEASANT;
		game.players[1].units[0].pos.x = 9;
		game.players[1].units[0].pos.y = 5;

		game.players[1].units[1].type = SOLDIER;
		game.players[1].units[1].pos.x = 6;
		game.players[1].units[1].pos.y = 6;



	game.map.resources = (resource*) malloc (2 * sizeof(resource));
		game.map.nResources = 2;

		game.map.resources[0].type = GOLD;
		game.map.resources[0].pos.x = 15;
		game.map.resources[0].pos.y = 13;

		game.map.resources[1].type = WOOD;
		game.map.resources[1].pos.x = 8;
		game.map.resources[1].pos.y = 8;



		//Display
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

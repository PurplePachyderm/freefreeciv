#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/engine/map_display.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

		//Camera init
	view camera;
	camera.offset.x = (SCREEN_WIDTH - MAP_SIZE*TILE_SIZE) / 2;	//Centers map for 1080p screen, 20*20 map and 50px tiles
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


	int quit = 0;
	int newEvent;	//Render new frame only if a new event has occured to save resources
	dispMap(window, renderer, sprites, texture, camera); //Would'nt be dsiplayed at first because of newEvent

	while(!quit){
		SDL_Delay(17);	//~60fps
		newEvent = cameraEvents(&event, &camera);
		if(newEvent){
			dispMap(window, renderer, sprites, texture, camera);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(sprites);
	return 0;
}

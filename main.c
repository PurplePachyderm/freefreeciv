#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/engine/map_display.h"


//Temporary main, for testing purpose only
int main(int argc, char** argv){

		//Camera init
	view camera;
	camera.offset.x = 585;	//Centers map for 1080p screen, 20*20 map and 50px tiles
	camera.offset.y = 165;
	camera.zoom = 1;
	camera.leftClick = 0;

		//SDL Initialization
	SDL_Window  * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(1920, 1080,
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
		SDL_Delay(25);	//40fps
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

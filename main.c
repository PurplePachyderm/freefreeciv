#include <stdio.h>
#include <stdlib.h>
#include "include/SDL2/SDL.h"
#include "include/engine/map_display.h"


int main(int argc, char** argv){

		//Ugly hard-coded test map
	int ** map;
	map = (int**) malloc(3 * sizeof(int**));
	for(int i=0; i<3; i++){
    	map[i] = (int*) malloc(3 * sizeof(int*));
	}

	map[0][0] = 1;
	map[0][1] = 0;
	map[0][2] = 1;

	map[1][0] = 0;
	map[1][1] = 1;
	map[1][2] = 0;

	map[2][0] = 1;
	map[2][1] = 0;
	map[2][2] = 1;

		//Camera init
	view camera;
	camera.offset.x = 300;
	camera.offset.y = 300;
	camera.zoom = 1;

		//SDL Initialization
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(1920, 1080,
								SDL_WINDOW_FULLSCREEN,
            					&window,
            					&renderer);

	//SDL_Event event;
	dispMap(window, renderer, map, camera);
	getchar();

	return 0;
}

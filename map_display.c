#include <stdio.h>
#include <stdlib.h>
#include "include/SDL2/SDL.h"
#include "include/engine/map_display.h"

void setRectangle(SDL_Rect * rect, int x, int y, int w, int h){
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

void dispMap(SDL_Window * window, SDL_Renderer * renderer, int ** map, view camera){
    SDL_Rect rect;
	int x;
	int y;
	int width;

		//Cleans Screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	setRectangle(&rect, 0, 0, 1920, 1080);
	SDL_RenderFillRect(renderer, &rect);

		//Draws tiles
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
                //Choosing color
            if(map[i][j] == 0){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);	//Red
            }
            else if(map[i][j] == 1){
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);	//Green
            }
                //Drawing rectangle
			x = (int) i*(50*camera.zoom) + camera.offset.x;
			y = (int) j*(50*camera.zoom) + camera.offset.y;
			width = (int) 50*camera.zoom;

            setRectangle(&rect, x, y, width, width);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}


void cameraEvents(SDL_Event * event, view * camera){	//Changes camera's zoom & offset
		//Camera motion
	if(event->type == SDL_MOUSEBUTTONDOWN
	&& event->button.button == SDL_BUTTON_LEFT){
		camera->leftClick = 1;
	}

	if(event->type == SDL_MOUSEBUTTONUP
	&& event->button.button == SDL_BUTTON_LEFT){
		camera->leftClick = 0;
	}

	if(event->type == SDL_MOUSEMOTION && camera->leftClick){
		camera->offset.x += event->motion.xrel;
		camera->offset.y += event->motion.yrel;

		//Prevents map to keep moving since x/yrel are preserved
		event->motion.xrel = 0;
		event->motion.yrel = 0;
	}

		//Camera zoom
	if(event->type == SDL_MOUSEWHEEL){
		camera->zoom += event->wheel.y * 0.25;
		event->wheel.y = 0;	//Idem

		//Zoom must be between 0.2 and 5
		if(camera->zoom < 0.2)
			camera->zoom = 0.2;
		if(camera->zoom > 5)
			camera->zoom = 5;
	}
}

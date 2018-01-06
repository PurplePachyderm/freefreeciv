#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../include/game/game.h"
#include "../include/display/display.h"

void setRectangle(SDL_Rect * rect, int x, int y, int w, int h){
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}



void blitSprite(SDL_Renderer * renderer,  SDL_Surface * sprites, SDL_Texture * texture, int xSprite, int ySprite, int x, int y, int width){
	// x/ySprite: Coords on sprites reference sheet
	// x/y: Coords on screen for rendering

	SDL_Rect  srcRect;
	SDL_Rect  destRect;
	setRectangle(&srcRect, 16*(xSprite-1), 16*(ySprite-1), 16, 16);
	setRectangle(&destRect, x, y, width, width);
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}



int cameraEvents(SDL_Event * event, view * camera){	//Changes camera's zoom & offset
	int newEvent = 0;	//If newEvent stays false, display won't be refreshed to save resources

	while(SDL_PollEvent(event)){
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
			newEvent = 1;


			camera->offset.x += event->motion.xrel;
			camera->offset.y += event->motion.yrel;

			//Map can't go out of the window
			if(camera->offset.x < -TILE_SIZE*camera->zoom*(MAP_SIZE+2))
				camera->offset.x = -TILE_SIZE*camera->zoom*(MAP_SIZE+2);
			if(camera->offset.x > SCREEN_WIDTH)
				camera->offset.x = SCREEN_WIDTH;
			if(camera->offset.y < -TILE_SIZE*camera->zoom*(MAP_SIZE+2))
				camera->offset.y = -TILE_SIZE*camera->zoom*(MAP_SIZE+2);
			if(camera->offset.y > SCREEN_HEIGHT)
				camera->offset.y = SCREEN_HEIGHT;

			//Prevents map to keep moving since x/yrel are preserved at end of event
			event->motion.xrel = 0;
			event->motion.yrel = 0;
		}

			//Camera zoom
		if(event->type == SDL_MOUSEWHEEL){
			newEvent = 1;

			float oldZoom = camera->zoom;

			camera->zoom += event->wheel.y * ZOOM_FACTOR;	//ZOOM_FACTOR Defines the speed of zoom
			event->wheel.y = 0;	//Idem

			//Zoom must be between 0.2 and 5
			if(camera->zoom < MIN_ZOOM)
				camera->zoom = MIN_ZOOM;
			if(camera->zoom > MAX_ZOOM)
				camera->zoom = MAX_ZOOM;

			//camera.offset is modified so the zoom is centered on the middle of the screen
			camera->offset.x = (int) camera->offset.x - ((camera->offset.x+(SCREEN_WIDTH/2-camera->offset.x)*(camera->zoom/oldZoom)) - SCREEN_WIDTH/2);
			camera->offset.y = (int) camera->offset.y - ((camera->offset.y+(SCREEN_HEIGHT/2-camera->offset.y)*(camera->zoom/oldZoom)) - SCREEN_HEIGHT/2);
		}
	}
	return newEvent;
}

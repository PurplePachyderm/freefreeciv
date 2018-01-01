#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "include/engine/map_display.h"


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



void dispMap(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera){
	int x;
	int y;
	int width;

		//Clear Screen
	SDL_Rect rect;
	SDL_SetRenderDrawColor(renderer, 66, 229, 244, 255);
	setRectangle(&rect, 0, 0, 1920, 1080);
	SDL_RenderFillRect(renderer, &rect);

		//Draw tiles
	coord sprite;	//Defines which sprite is gonna be used (see sprites reference sheet)
	for(int i = 0; i<MAP_SIZE; i++){
        for(int j = 0; j<MAP_SIZE; j++){

				//For coasts and corners
			if(i == 0 || j==0 || i == MAP_SIZE-1 || j == MAP_SIZE-1){
				x = (int) i*(TILE_SIZE*camera.zoom) + camera.offset.x;
				y = (int) j*(TILE_SIZE*camera.zoom) + camera.offset.y;
				width = (int) TILE_SIZE*camera.zoom + 1;
				blitSprite(renderer, sprites, texture, 2, 25, x, y, width);	//Blits sea underneath coast
			}
				//Corner
			if(i == 0 && j == 0){
				sprite.x = 1;	//Left top corner
				sprite.y = 29;
			}
			else if(i == MAP_SIZE-1 && j == 0){
				sprite.x = 3;	//Right top corner
				sprite.y = 29;
			}
			else if(i == 0 && j == MAP_SIZE-1){
				sprite.x = 1;	//Left bottom corner
				sprite.y = 31;
			}
			else if(i == MAP_SIZE-1 && j == MAP_SIZE-1){
				sprite.x = 3;	//Right bottom corner
				sprite.y = 31;
			}
				//Coast
			else if(j == 0){
				sprite.x = 2;	//Top coast
				sprite.y = 29;
			}
			else if(i == MAP_SIZE-1){
				sprite.x = 3;	//Right coast
				sprite.y = 30;
			}
			else if(j == MAP_SIZE-1){
				sprite.x = 2;	//Bottom coast
				sprite.y = 31;
			}
			else if(i == 0){
				sprite.x = 1;	//Left coast
				sprite.y = 30;
			}
				//Inside tile
			else if((i+j+1)%2 == 0){
				sprite.x = 3;	//Dark grass
				sprite.y = 1;
            }
            else{
				sprite.x = 4;	//Light Grass
				sprite.y = 1;
            }


			x = (int) i*(TILE_SIZE*camera.zoom) + camera.offset.x;
			y = (int) j*(TILE_SIZE*camera.zoom) + camera.offset.y;
			width = (int) TILE_SIZE*camera.zoom + 1;	//The +1 avoids 1 pixel gap between tiles on particular zoom values
			blitSprite(renderer, sprites, texture, sprite.x, sprite.y, x, y, width);
        }
    }
    SDL_RenderPresent(renderer);
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
			if(camera->offset.x < -50*camera->zoom*MAP_SIZE)
				camera->offset.x = -50*camera->zoom*MAP_SIZE;
			if(camera->offset.x > 1920)
				camera->offset.x = 1920;
			if(camera->offset.y < -50*camera->zoom*MAP_SIZE)
				camera->offset.y = -50*camera->zoom*MAP_SIZE;
			if(camera->offset.y > 1080)
				camera->offset.y = 1080;

			//Prevents map to keep moving since x/yrel are preserved at end of event
			event->motion.xrel = 0;
			event->motion.yrel = 0;
		}

			//Camera zoom
		if(event->type == SDL_MOUSEWHEEL){
			newEvent = 1;

			camera->zoom += event->wheel.y * 0.25;
			event->wheel.y = 0;	//Idem

			//Zoom must be between 0.2 and 5
			if(camera->zoom < MIN_ZOOM)
				camera->zoom = MIN_ZOOM;
			if(camera->zoom > MAX_ZOOM)
				camera->zoom = MAX_ZOOM;
		}
	}
	return newEvent;
}

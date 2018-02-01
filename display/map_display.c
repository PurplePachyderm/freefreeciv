#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../include/game/game.h"
#include "../include/display/map_display.h"

	//Blits water sprites all over the screen
void dispBackground(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera){
	int width = (int) TILE_SIZE*camera.zoom + 1;
	coord currentPos;

		//Blitting starts on top left corner, and is done line per line, stops when reaching of screen
	currentPos.y = camera.offset.y%width - width;	//Relative to camera offset, so it's aligned with continent tiles
	while(currentPos.y < SCREEN_HEIGHT){

		currentPos.x = camera.offset.x%width - width;
		while(currentPos.x < SCREEN_WIDTH){

			width = (int) TILE_SIZE*camera.zoom + 1;	//The +1 avoids 1 pixel gap between tiles on particular zoom values

				//if statement avoids blitting sprites under the continent
			if( (currentPos.x < camera.offset.x+width || currentPos.x > camera.offset.x+(MAP_SIZE)*width) ||
			((currentPos.y < camera.offset.y+width || currentPos.y > camera.offset.y+(MAP_SIZE)*width)) ){
				blitSprite(renderer, sprites, texture, 2, 25, currentPos.x, currentPos.y, width);
			}

			currentPos.x += width;
		}
		currentPos.y += width;
	}
}


	//Displays sea+continent
void dispContinent(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera){
	int x;
	int y;
	int width;

		//Draw tiles
	coord sprite;	//Defines which sprite is gonna be used (see sprites reference sheet)
	for(int i = 0; i<MAP_SIZE+2; i++){
        for(int j = 0; j<MAP_SIZE+2; j++){

				//Corner
			if(i == 0 && j == 0){
				sprite.x = 1;	//Left top corner
				sprite.y = 29;
			}
			else if(i == MAP_SIZE+1 && j == 0){
				sprite.x = 3;	//Right top corner
				sprite.y = 29;
			}
			else if(i == 0 && j == MAP_SIZE+1){
				sprite.x = 1;	//Left bottom corner
				sprite.y = 31;
			}
			else if(i == MAP_SIZE+1 && j == MAP_SIZE+1){
				sprite.x = 3;	//Right bottom corner
				sprite.y = 31;
			}
				//Coast
			else if(j == 0){
				sprite.x = 2;	//Top coast
				sprite.y = 29;
			}
			else if(i == MAP_SIZE+1){
				sprite.x = 3;	//Right coast
				sprite.y = 30;
			}
			else if(j == MAP_SIZE+1){
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

			if( (x+width>0 && y+width>0) || (x<SCREEN_WIDTH && y<SCREEN_HEIGHT) ){	//if statement avoids blitting sprites out of the screen
				blitSprite(renderer, sprites, texture, sprite.x, sprite.y, x, y, width);
			}
        }
    }
}



void dispMap(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera){
	dispBackground(renderer, sprites, texture, camera);
	dispContinent(renderer, sprites, texture, camera);
}

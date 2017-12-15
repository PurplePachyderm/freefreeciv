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
	int wh;

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
			wh = (int) 50*camera.zoom;

			printf("Coords:\n");
			printf("x: %d\n", x);
			printf("y: %d\n", y);
			printf("w/h : %d\n", wh);
            setRectangle(&rect, x, y, wh, wh);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

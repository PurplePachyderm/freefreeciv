#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/hud.h"
#include "../include/display/display.h"
#include "../include/display/map_display.h"
#include "../include/display/tokens_display.h"
#include "../include/display/menu.h"

#include "../include/coord.h"
#include "../include/game/game.h"
#include "../include/game/units_actions.h"



//Main HUD (default game state)
void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game){
	SDL_Event event;
    int quit = 0;
    int newEvent = 0;

    view camera;
    camera.offset.x = (SCREEN_WIDTH - (MAP_SIZE+2)*TILE_SIZE) / 2;	//Centers map
    camera.offset.y = (SCREEN_HEIGHT - (MAP_SIZE+2)*TILE_SIZE) / 2;
    camera.zoom = 1;
    camera.leftClick = 0;

    coord exitPos;  //Exit menu cross (right top corner)
    exitPos.x = SCREEN_WIDTH - TILE_SIZE;
    exitPos.y = 0;


    dispMap(renderer, sprites, texture, camera); //Would'nt be displayed at first because of newEvent
    dispTokens(renderer, sprites, texture, camera, game);
    blitSprite(renderer, sprites, texture, 5, 30, exitPos.x, exitPos.y, TILE_SIZE);

    SDL_Rect srcRect;
    setRectangle(&srcRect, 0, 0, 3840, 2160); //Dim of background

    SDL_Rect destRect;
    setRectangle(&destRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_RenderPresent(renderer);


    while(!quit){
        SDL_Delay(REFRESH_PERIOD);

        while(SDL_PollEvent(&event)){
            newEvent = events(event, &camera);
            switch(newEvent){
                case MENU:
                    quit = inGameMenu(renderer, sprites, texture, game);
                    break;
            }
        }

        if(newEvent){   //Refresh display if a new event has occured
            dispMap(renderer, sprites, texture, camera);
            dispTokens(renderer, sprites, texture, camera, game);
            blitSprite(renderer, sprites, texture, 5, 30, exitPos.x, exitPos.y, TILE_SIZE); //Menu cross
            SDL_RenderPresent(renderer);
        }
    }
}

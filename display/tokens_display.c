#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../include/game/game.h"
#include "../include/display/tokens_display.h"


void dispUnits(SDL_Renderer * renderer, SDL_Texture * texture, view camera, player player){
    coord sprite;

    int x;
    int y;
    int width;

    for(int i=0; i<player.nUnits; i++){

        switch(player.units[i].type){

            case PEASANT:
                switch(player.id){

                    case RED:
                        sprite.x = 3;   //Red peasant
                        sprite.y = 37;
                        break;

                    case BLUE:
                        sprite.x = 3;   //Blue peasant
                        sprite.y = 36;
                        break;

                    case GREEN:
                        sprite.x = 5;   //Green peasant
                        sprite.y = 38;
                        break;

                    case YELLOW:
                        sprite.x = 3;   //Yellow peasant
                        sprite.y = 38;
                }
                break;


            case SOLDIER:
                switch(player.id){

                    case RED:
                        sprite.x = 3;   //Red soldier
                        sprite.y = 18;
                        break;

                    case BLUE:
                        sprite.x = 3;   //Blue soldier
                        sprite.y = 17;
                        break;

                    case GREEN:
                        sprite.x = 3;   //Green soldier
                        sprite.y = 20;
                        break;

                    case YELLOW:
                        sprite.x = 3;   //Yellow soldier
                        sprite.y = 19;
                }
        }

        x = (int) player.units[i].pos.x * (TILE_SIZE*camera.zoom) + camera.offset.x;
        y = (int) player.units[i].pos.y * (TILE_SIZE*camera.zoom) + camera.offset.y;
        width = (int) TILE_SIZE*camera.zoom + 1;	//The +1 avoids 1 pixel gap between tiles on particular zoom values

        if( (x+width>0 && y+width>0) || (x<SCREEN_WIDTH && y<SCREEN_HEIGHT) ){	//if statement avoids blitting sprites out of the screen
            blitSprite(renderer, texture, sprite.x, sprite.y, x, y, width);
        }
    }
}



void dispBuildings(SDL_Renderer * renderer, SDL_Texture * texture, view camera, player player){
    coord sprite;

    int x;
    int y;
    int width;

    for(int i=0; i<player.nBuildings; i++){

        switch(player.buildings[i].type){

            case CITY:
                switch(player.id){

                    case RED:
                        sprite.x = 3;   //Red city
                        sprite.y = 5;
                        break;

                    case BLUE:
                        sprite.x = 5;   //Blue city
                        sprite.y = 5;
                        break;

                    case GREEN:
                        sprite.x = 4;   //Green city
                        sprite.y = 5;
                        break;

                    case YELLOW:
                        sprite.x = 6;   //Yellow city
                        sprite.y = 5;
                }
                break;


            case BARRACK:
                switch(player.id){

                    case RED:
                        sprite.x = 3;   //Red barrack
                        sprite.y = 6;
                        break;

                    case BLUE:
                        sprite.x = 5;   //Blue barrack
                        sprite.y = 6;
                        break;

                    case GREEN:
                        sprite.x = 4;   //Green barrack
                        sprite.y = 6;
                        break;

                    case YELLOW:
                        sprite.x = 6;   //Yellow barrack
                        sprite.y = 6;
                }
        }

        x = (int) player.buildings[i].pos.x * (TILE_SIZE*camera.zoom) + camera.offset.x;
        y = (int) player.buildings[i].pos.y * (TILE_SIZE*camera.zoom) + camera.offset.y;
        width = (int) TILE_SIZE*camera.zoom + 1;	//The +1 avoids 1 pixel gap between tiles on particular zoom values

        if( (x+width>0 && y+width>0) || (x<SCREEN_WIDTH && y<SCREEN_HEIGHT) ){	//if statement avoids blitting sprites out of the screen
            blitSprite(renderer, texture, sprite.x, sprite.y, x, y, width);
        }
    }
}



void dispPlayer(SDL_Renderer * renderer, SDL_Texture * texture, view camera, player player){
    dispUnits(renderer, texture, camera, player);
    dispBuildings(renderer, texture, camera, player);
}



void dispResources(SDL_Renderer * renderer, SDL_Texture * texture, view camera, struct map map){
    coord sprite;

    int x;
    int y;
    int width;

    for(int i=0; i<map.nResources; i++){
        switch(map.resources[i].type){
            case GOLD:
                sprite.x = 7;
                sprite.y = 30;
                break;

            case WOOD:
                sprite.x = 6;
                sprite.y = 1;
        }

        x = (int) map.resources[i].pos.x * (TILE_SIZE*camera.zoom) + camera.offset.x;
        y = (int) map.resources[i].pos.y * (TILE_SIZE*camera.zoom) + camera.offset.y;
        width = (int) TILE_SIZE*camera.zoom + 1;	//The +1 avoids 1 pixel gap between tiles on particular zoom values

        if( (x+width>0 && y+width>0) || (x<SCREEN_WIDTH && y<SCREEN_HEIGHT) ){	//if statement avoids blitting sprites out of the screen
            blitSprite(renderer, texture, sprite.x, sprite.y, x, y, width);
        }
    }
}



void dispTokens(SDL_Renderer * renderer, SDL_Texture * texture, view camera, struct game game){
    dispResources(renderer, texture, camera, game.map);

    for(int i=0; i<game.nPlayers; i++){
        dispBuildings(renderer, texture, camera, game.players[i]);
        dispUnits(renderer, texture, camera, game.players[i]);
    }
}

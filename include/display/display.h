#pragma once

#include "../coord.h"
#include "../game/game.h"

//Essential macros/functions for display, game and camera events management

    //Macros
#define REFRESH_PERIOD 17
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define TILE_SIZE 50

#define MAX_ZOOM 5
#define MIN_ZOOM 0.8

#define ZOOM_FACTOR 0.25

    //Event macros (defines types of events)
#define MENU 1
#define TILE_SELECTION 2
#define CAMERA 3
#define END_TURN 4


    //Structures
typedef struct{
    coord offset;
    float zoom;
    int leftClick;
}view;


//Prototypes
void setRectangle(SDL_Rect * rect, int x, int y, int w, int h);
void blitSprite(SDL_Renderer * renderer,  SDL_Surface * sprites, SDL_Texture * texture, int xSprite, int ySprite, int x, int y, int width);

int events(SDL_Event event, view *camera, game game, coord * selectedTile);

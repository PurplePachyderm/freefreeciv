#include "engine.h"

    //Macros
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define TILE_SIZE 50

#define MAX_ZOOM 5
#define MIN_ZOOM 0.8

#define ZOOM_FACTOR 0.25


    //Structures
typedef struct{
    coord offset;
    float zoom;
	int leftClick;
}view;


    //Prototypes
void setRectangle(SDL_Rect * rect, int x, int y, int w, int h);
void blitSprite(SDL_Renderer * renderer,  SDL_Surface * sprites, SDL_Texture * texture, int xSprite, int ySprite, int x, int y, int width);

void dispBackground(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera);
void dispContinent(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera);
void dispMap(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera);

int cameraEvents(SDL_Event * event, view * camera);

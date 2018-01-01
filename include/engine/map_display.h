#define TILE_SIZE 50
#define MAP_SIZE 15


typedef struct{
    int x;
    int y;
}coord;

typedef struct{
    coord offset;
    float zoom;
	int leftClick;
}view;


void setRectangle(SDL_Rect * rect, int x, int y, int w, int h);
void blitSprite(SDL_Renderer * renderer,  SDL_Surface * sprites, SDL_Texture * texture, int xSprite, int ySprite, int x, int y, int width);
void dispMap(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, view camera);
int cameraEvents(SDL_Event * event, view * camera);

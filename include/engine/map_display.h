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
void dispMap(SDL_Window * window, SDL_Renderer * renderer, int ** map, view camera);
void cameraEvents(SDL_Event * event, view * camera);

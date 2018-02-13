//Contains display of different Huds in game (called in Hud functions)
#include "display.h"
#include "../game/game.h"

SDL_Rect basicDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, char * message);

SDL_Rect mainDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown);
SDL_Rect peasantDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int peasantId);
SDL_Rect soldierDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
SDL_Rect buildingDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int soldierId);
SDL_Rect targetDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int isMovement, coord pos);
SDL_Rect foreignDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int ownerId, int tokenId, int isUnit);

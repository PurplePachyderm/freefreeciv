#include "easywsclient.hpp"
#include "../game/game.h"
#include "../display/display.h"


int mMainHud(easywsclient::WebSocket * ws, room room, SDL_Renderer * renderer, SDL_Texture * texture, struct game game);

int mPlayerHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera);

int mPeasantHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int peasantId);
int mSoldierHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int soldierId);
int mBuildingHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int buildingId);

int mEnemyPlayerHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int ownTurn);

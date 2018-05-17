#include "../include/multiplayer/easywsclient.hpp"
#include "../include/game/game.h"

int mMainHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game game);

int mPlayerHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera);
void mAIHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera);

int mPeasantHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int peasantId);
int mSoldierHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int soldierId);
int mBuildingHud(easywsclient::WebSocket * ws, room room,(SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int buildingId);
int mTargetHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int isMovement, coord pos, coord * target);
int mForeignHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera, int * countdown, int * countdownSec, int ownerId, int tokenId, int isUnit);

int mEnemyPlayerHud(easywsclient::WebSocket * ws, room room,SDL_Renderer * renderer, SDL_Texture * texture, struct game * game, view * camera);

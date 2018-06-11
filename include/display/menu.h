#include <SDL2/SDL_mixer.h>

//Macros for slots
#define EMPTY 0
#define HUMAN 1
#define AI_SLOT 2


void mainMenu(SDL_Renderer * renderer, SDL_Texture * texture);

int newGameMenu(SDL_Renderer * renderer, SDL_Texture * texture, Mix_Music * music);
int cycleSlot(int * slots, int slotId, int firstIteration);

int loadSaveMenu(SDL_Renderer * renderer, SDL_Texture * texture, Mix_Music * music, struct game * game);
char ** getSaves(int * nSaves);
int createSaveMenu(SDL_Renderer * renderer, struct game game);

int inGameMenu(SDL_Renderer * renderer, struct game game);

void splashScreen(SDL_Renderer * renderer);

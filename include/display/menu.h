//Macros for slots
#define EMPTY 0
#define HUMAN 1
#define AI_SLOT 2


void mainMenu(SDL_Renderer * renderer, SDL_Texture * texture);

int newGameMenu(SDL_Renderer * renderer, SDL_Texture * texture);
int cycleSlot(int * slots, int slotId, int firstIteration);

int inGameMenu(SDL_Renderer * renderer);

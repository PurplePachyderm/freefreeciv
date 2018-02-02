#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/menu.h"
#include "../include/display/display.h"

void menu()
{
	srand(time(NULL));
	SDL_Window* WINDOW;
	SDL_Renderer* RENDERER;
	SDL_Surface* IMAGE;
	int flag = IMG_INIT_JPG;
	IMG_Init(flag);
	int quit=0;
	IMAGE=IMG_Load("menu.jpeg");
	SDL_Texture * texture = NULL;
	texture = SDL_CreateTextureFromSurface(RENDERER, IMAGE);
	SDL_RenderCopy(RENDERER, texture, NULL, NULL);

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_FULLSCREEN,&WINDOW,&RENDERER);
	SDL_SetRenderDrawColor(RENDERER,255,255,255,255);
	SDL_Event EVENT;
	do
	{
		aff_carre(RENDERER,100,100,1400,200);
		aff_carre(RENDERER,750,400,100,100);
		aff_carre(RENDERER,750,550,100,100);
		aff_carre(RENDERER,750,700,100,100);
		SDL_Delay(100);
		SDL_PollEvent(&EVENT);
		if(EVENT.type==SDL_MOUSEBUTTONDOWN && EVENT.button.button==SDL_BUTTON_LEFT)
		{
			if(((EVENT.button.x>750)&&(EVENT.button.x<850))&&((EVENT.button.y>400)&&(EVENT.button.y<500)))
			{
				change_couleur(RENDERER); //Renvoie la boucle de jeu
			}
			else if(((EVENT.button.x>750)&&(EVENT.button.x<850))&&((EVENT.button.y>550)&&(EVENT.button.y<650)))
			{
				change_couleur(RENDERER); //Renvoie les options
			}
			else if(((EVENT.button.x>750)&&(EVENT.button.x<850))&&((EVENT.button.y>700)&&(EVENT.button.y<800)))
			{
				quit=1; //Quitte le programme
			}
		}
		SDL_RenderPresent(RENDERER);
	}while(quit==0);
	SDL_FreeSurface(IMAGE);
}

void aff_carre(SDL_Renderer* RENDERER, int x, int y, int w, int h)
{
	SDL_Rect RECT;
	RECT.x=x;
	RECT.y=y;
	RECT.w=w;
	RECT.h=h;

	SDL_RenderFillRect(RENDERER,&RECT);
}

void change_couleur(SDL_Renderer* RENDERER)
{
	int x=0, y=0, z=0;
	x=rand()%256;
	y=rand()%256;
	z=rand()%256;
	SDL_SetRenderDrawColor(RENDERER,x,y,z,255);
}



//Menu HUD (Quit game, load & save)
//XXX Move to menu.c in other versions?
int inGameMenu(SDL_Renderer * renderer, SDL_Surface * sprites,  SDL_Texture * texture, game game){
	//BUG Segmentation fault in the function (happens when exitting function)
	//We're gonna Valgrind that shit

    SDL_Event event;
    int quit = 0;
    int exitGame = 0; //Return value

    //Font size and surface height are different, but we need to locate the actual text for hitboxes
    float fontFactor = 0.655;


    //Background
    SDL_Rect srcRect;
    setRectangle(&srcRect, 0, 0, 3840, 2160); //Dim of background

    SDL_Rect destRect;
    setRectangle(&destRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface * background = IMG_Load("resources/menu.png");
    SDL_Texture * backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);

    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(background);

    //Title
    TTF_Font * font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/8);
    SDL_Color color = {255, 237, 43};

    SDL_Surface * title= TTF_RenderText_Blended(font, "freefreeciv", color);
    SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, title);

    setRectangle(&srcRect, 0, title->h - (title->h*fontFactor+1), title->w, title->h * fontFactor + 1);
    setRectangle(&destRect, SCREEN_WIDTH/2 - title->w/2, 3*SCREEN_HEIGHT/64, title->w, title->h * fontFactor + 1);
    SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);


    //Save
    font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/16);

    SDL_Surface * save = TTF_RenderText_Blended(font, "save", color);
    textTexture = SDL_CreateTextureFromSurface(renderer, save);

    setRectangle(&srcRect, 0, save->h - (save->h*fontFactor+1), save->w, save->h * fontFactor + 1);
    setRectangle(&destRect, SCREEN_WIDTH/2 - save->w/2, 3*SCREEN_HEIGHT/8-((save->h*fontFactor+1)/2), save->w, save->h * fontFactor + 1);
    SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

    SDL_DestroyTexture(textTexture);


    //Load
    SDL_Surface * load = TTF_RenderText_Blended(font, "load", color);
    textTexture = SDL_CreateTextureFromSurface(renderer, load);

    setRectangle(&srcRect, 0, load->h - (load->h*fontFactor+1), load->w, load->h * fontFactor + 1);
    setRectangle(&destRect, SCREEN_WIDTH/2 - load->w/2, 2*SCREEN_HEIGHT/4-((load->h*fontFactor+1)/2), load->w, load->h * fontFactor + 1);
    SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_DestroyTexture(textTexture);


    //Quit game
    SDL_Surface * quitGame = TTF_RenderText_Blended(font, "quit", color);
    textTexture = SDL_CreateTextureFromSurface(renderer, quitGame);

    setRectangle(&srcRect, 0, quitGame->h - (quitGame->h*fontFactor+1), quitGame->w, quitGame->h * fontFactor + 1);
    setRectangle(&destRect, SCREEN_WIDTH/2 - quitGame->w/2, 5*SCREEN_HEIGHT/8-((quitGame->h*fontFactor+1)/2), quitGame->w, quitGame->h * fontFactor + 1);
    SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_DestroyTexture(textTexture);

    //Exit menu
    coord exitPos;  //Right top corner
    exitPos.x = SCREEN_WIDTH - TILE_SIZE;
    exitPos.y = 0;
    blitSprite(renderer, sprites, texture, 5, 30, exitPos.x, exitPos.y, TILE_SIZE);

    SDL_RenderPresent(renderer);
	TTF_CloseFont(font);

    //Loop (display doesn't change anymore)
    while(!quit){
        SDL_Delay(REFRESH_PERIOD);

        while(SDL_PollEvent(&event)){
            //TODO Add all events
            //Quit menu (cross)
            if(event.type == SDL_MOUSEBUTTONDOWN
        	&& event.button.button == SDL_BUTTON_LEFT
        	&& event.button.x >= SCREEN_WIDTH - TILE_SIZE && event.button.y <= TILE_SIZE){
                quit = 1;
            }
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                quit = 1;
            }

			//Quit game ("Quit" button)
			if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - quitGame->w/2 && event.button.x <= SCREEN_WIDTH/2 + quitGame->w/2
			&& event.button.y >= 5*SCREEN_HEIGHT/8-((quitGame->h*fontFactor+1)/2) && event.button.y <= 5*SCREEN_HEIGHT/8-((quitGame->h*fontFactor+1)/2)+quitGame->h * fontFactor + 1){
				quit = 1;
				exitGame = 1;
			}
        }
    }

	//Needed for hitboxes so freed at end of function
	SDL_FreeSurface(background);
	SDL_FreeSurface(title);
	SDL_FreeSurface(save);
	SDL_FreeSurface(load);
	SDL_FreeSurface(quitGame);

    return exitGame;
}

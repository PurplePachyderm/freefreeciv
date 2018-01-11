#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

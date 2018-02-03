#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/hud.h"
#include "../include/display/display.h"
#include "../include/display/map_display.h"
#include "../include/display/tokens_display.h"
#include "../include/display/menu.h"

#include "../include/coord.h"
#include "../include/game/game.h"
#include "../include/game/units_actions.h"



//Main HUD (default game state)
SDL_Rect mainHudDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown){
	float fontFactor = 0.9;	//Font opening size and surface size are different
	SDL_Rect srcRect;
	SDL_Rect destRect;

	//Map & tokens
    dispMap(renderer, sprites, texture, camera); //Would'nt be displayed at first because of newEvent
    dispTokens(renderer, sprites, texture, camera, game);


	//Menu cross
	coord exitPos;  //Exit menu cross (right top corner)
	exitPos.x = SCREEN_WIDTH - TILE_SIZE;
	exitPos.y = 0;

	blitSprite(renderer, sprites, texture, 5, 30, exitPos.x, exitPos.y, TILE_SIZE);


	//Player indicator
	SDL_Color colors [4] = {{224, 26, 26},	//Red
	{55, 131, 206},	//Blue
	{40, 175, 63},	//Green
	{255, 228, 58}};	//Yellow

	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", SCREEN_HEIGHT/36);
	//TTF_SetFontOutline(font, 2);

	char playerText [9];
	sprintf(playerText, "PLAYER %d", game.currentPlayer+1);

	SDL_Surface * player = TTF_RenderText_Blended(font, playerText, colors[game.currentPlayer]);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, player);

	setRectangle(&srcRect, 0, player->h-(player->h * fontFactor+1), player->w, player->h * fontFactor+1);
	setRectangle(&destRect, 4, 0, player->w, player->h * fontFactor+1);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_DestroyTexture(textTexture);


	//TODO Countdown display
	SDL_Color white = {255, 255, 255};

	char countdownText [4];
	sprintf(countdownText, "%d", countdown);

	SDL_Surface * countdownSurf = TTF_RenderText_Blended(font, countdownText, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, countdownSurf);

	setRectangle(&srcRect, 0, countdownSurf->h-(countdownSurf->h * fontFactor+1), countdownSurf->w, countdownSurf->h * fontFactor+1);
	setRectangle(&destRect, 4, countdownSurf->h * fontFactor+1,
	countdownSurf->w, countdownSurf->h * fontFactor+1);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_DestroyTexture(textTexture);


	//Finish turn
	SDL_Surface * end = TTF_RenderText_Blended(font, "END TURN", white);
	textTexture = SDL_CreateTextureFromSurface(renderer, end);

	setRectangle(&srcRect, 0, player->h-(player->h * fontFactor+1), end->w, end->h * fontFactor);
	setRectangle(&destRect, SCREEN_WIDTH-end->w, SCREEN_HEIGHT-(end->h*fontFactor)-4, end->w, end->h * fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);


	SDL_FreeSurface(player);
	SDL_FreeSurface(countdownSurf);
	SDL_FreeSurface(end);
	SDL_DestroyTexture(textTexture);

	TTF_CloseFont(font);
    SDL_RenderPresent(renderer);

	return destRect;	//Coords of 'End turn' button (needed for event)
}


void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game){
	SDL_Event event;
    int quit = 0;
    int newEvent = 0;
	int countdown = TURN_TIME * 1000;	//30 sec in ms
	int countdownSec = 89; //Approx in sec for display

    view camera;
    camera.offset.x = (SCREEN_WIDTH - (MAP_SIZE+2)*TILE_SIZE) / 2;	//Centers map
    camera.offset.y = (SCREEN_HEIGHT - (MAP_SIZE+2)*TILE_SIZE) / 2;
    camera.zoom = 1;
    camera.leftClick = 0;

	//First display before any event
	mainHudDisplay(renderer, sprites, texture, game, camera, countdownSec);

    while(!quit){
        SDL_Delay(REFRESH_PERIOD);

        while(SDL_PollEvent(&event)){
            newEvent = events(event, &camera);
            switch(newEvent){
                case MENU:
                    quit = inGameMenu(renderer, sprites, texture, game);
                    break;
            }
        }

		countdown -= REFRESH_PERIOD;
		if(countdown/1000 < countdownSec){	//Refresh display
			countdownSec--;
			newEvent = 1;

			if(countdownSec < 0){	//Change player at end of countdown
				countdown = TURN_TIME * 1000;
				countdownSec = 89;
				game.currentPlayer = (game.currentPlayer+1) % game.nPlayers;
			}
		}

        if(newEvent){  //Refresh display if a new event has occured
			mainHudDisplay(renderer, sprites, texture, game, camera, countdownSec);
			newEvent = 0;
		}
    }
}

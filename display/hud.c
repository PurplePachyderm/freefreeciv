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


//Basic display (map, player, countdown, cross)
SDL_Rect basicDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, char * message){
	//Warning: Does not include RenderPresent

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

	//Countdown
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
	SDL_Surface * end = TTF_RenderText_Blended(font, message, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, end);

	setRectangle(&srcRect, 0, player->h-(player->h * fontFactor+1), end->w, end->h * fontFactor);
	setRectangle(&destRect, SCREEN_WIDTH-end->w, SCREEN_HEIGHT-(end->h*fontFactor)-4, end->w, end->h * fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);


	SDL_FreeSurface(player);
	SDL_FreeSurface(countdownSurf);
	SDL_FreeSurface(end);
	SDL_DestroyTexture(textTexture);

	TTF_CloseFont(font);

	return destRect;	//Coords of 'End turn' button (needed for event)
}



//Main HUD (default game state)
SDL_Rect mainDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown){
	SDL_Rect turnRect = basicDisplay(renderer, sprites, texture, game, camera, countdown, "END TURN");

	//TODO Resources indicator

	SDL_RenderPresent(renderer);

	return turnRect;
}



void mainHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game){
	SDL_Event event;
    int quit = 0;
    int newEvent = 0;

	coord selectedTile;
	int tokenId;

	int countdown = TURN_TIME * 1000;	//30 sec in ms
	int countdownSec = TURN_TIME; //Approx in sec for display

	//int quitGame;

    view camera;
    camera.offset.x = (SCREEN_WIDTH - (MAP_SIZE+2)*TILE_SIZE) / 2;	//Centers map
    camera.offset.y = (SCREEN_HEIGHT - (MAP_SIZE+2)*TILE_SIZE) / 2;
    camera.zoom = 1;
    camera.leftClick = 0;

	//First display before any event
	SDL_Rect endButton = mainDisplay(renderer, sprites, texture, game, camera, countdownSec);

    while(!quit){
        SDL_Delay(REFRESH_PERIOD);

        while(SDL_PollEvent(&event)){
            newEvent = events(event, &camera, game, &selectedTile);

			//End turn (potentially overrides TILE_SELECTION)
			if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= endButton.x && event.button.x <= endButton.x + endButton.w
			&& event.button.y >= endButton.y && event.button.y <= endButton.y + endButton.h){
				newEvent = END_TURN;
				countdown = TURN_TIME * 1000;	//Change current player
				countdownSec = TURN_TIME;
				game.currentPlayer = (game.currentPlayer+1) % game.nPlayers;
			}

            switch(newEvent){
                case MENU:
                    quit = inGameMenu(renderer, sprites, texture);
                    break;

				case TILE_SELECTION:
					tokenId = checkOwnUnit(game, selectedTile);
					if(tokenId < game.players[game.currentPlayer].nUnits){
						switch(game.players[game.currentPlayer].units[tokenId].type){

							case PEASANT:
							quit = peasantHud(renderer, sprites, texture, &game, &camera, &countdown, &countdownSec, tokenId);
							break;

							case SOLDIER:
							//SoldierHud
							printf("Clicked on soldier\n");
							break;
						}
					}
					else{
						tokenId = checkOwnBuilding(game, selectedTile);
						if(tokenId < game.players[game.currentPlayer].nBuildings){
							switch(game.players[game.currentPlayer].buildings[tokenId].type){

								case CITY:
								//CityHud
								printf("Clicked on city\n");
								break;

								case BARRACK:
								//barrackHud
								printf("Clicked on barrack\n");
								break;
							}
						}
					}
            }
        }

		countdown -= REFRESH_PERIOD;
		if(countdown/1000 < countdownSec){	//Refresh display
			countdownSec--;
			newEvent = 1;

			if(countdownSec < 0){	//Change player at end of countdown
				countdown = TURN_TIME * 1000;
				countdownSec = TURN_TIME;
				game.currentPlayer = (game.currentPlayer+1) % game.nPlayers;
			}
		}

        if(newEvent){  //Refresh display if a new event has occured
			mainDisplay(renderer, sprites, texture, game, camera, countdownSec);
			newEvent = 0;
		}
    }
}



//Peasant Hud
SDL_Rect peasantDisplay(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game game, view camera, int countdown, int peasantId){
	SDL_Rect cancelRect = basicDisplay(renderer, sprites, texture, game, camera, countdown, "CANCEL");

	//Highlighter
	coord highlighterCoords;
	highlighterCoords.x = camera.offset.x+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[peasantId].pos.x);
	highlighterCoords.y = camera.offset.y+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[peasantId].pos.y);
	blitSprite(renderer, sprites, texture, game.currentPlayer+4, 43, highlighterCoords.x, highlighterCoords.y, TILE_SIZE*camera.zoom);

	SDL_RenderPresent(renderer);
	return cancelRect;
}



int peasantHud(SDL_Renderer * renderer, SDL_Surface * sprites, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int peasantId){
	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	//int tokenId;
	//int ownerId;

	SDL_Rect cancelRect = peasantDisplay(renderer, sprites, texture, *game, *camera, *countdownSec, peasantId);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, sprites, texture);
				if(quitGame)
					quit = 1;
			}

			//Cancel button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>cancelRect.x && event.button.x<cancelRect.x+cancelRect.w
			&& event.button.y>cancelRect.y && event.button.y<cancelRect.y+cancelRect.y){
				quit = 1;
			}
		}

		//Countdown
		*countdown -= REFRESH_PERIOD;
		if(*countdown/1000 < *countdownSec){	//Refresh display
			(*countdownSec)--;	// (╯°□°）╯︵ ┻━┻ Don't forget the parenthesis
			newEvent = 1;

			if(*countdownSec < 0){	//Change player at end of countdown
				*countdown = TURN_TIME * 1000;
				*countdownSec = TURN_TIME;
				game->currentPlayer = (game->currentPlayer+1) % game->nPlayers;
				quit = 1;	//Back to mainHub for the next player
			}
		}

		//Refresh
		if(newEvent){
			peasantDisplay(renderer, sprites, texture, *game, *camera, *countdownSec, peasantId);
		}
	}

	return quitGame;
}

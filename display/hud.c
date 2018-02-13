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


//Basic functions (reused in several Huds)
SDL_Rect basicDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, char * message){
	//Warning: Does not include RenderPresent
	//Renders map, player indocator, countdown,

	float fontFactor = 0.9;	//Font opening size and surface size are different
	SDL_Rect srcRect;
	SDL_Rect destRect;


	//Map & tokens
    dispMap(renderer, texture, camera); //Would'nt be displayed at first because of newEvent
    dispTokens(renderer, texture, camera, game);


	//Menu cross
	coord exitPos;  //Exit menu cross (right top corner)
	exitPos.x = SCREEN_WIDTH - TILE_SIZE;
	exitPos.y = 0;

	blitSprite(renderer, texture, 5, 30, exitPos.x, exitPos.y, TILE_SIZE);


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

	SDL_FreeSurface(player);
	SDL_DestroyTexture(textTexture);


	//Countdown
	SDL_Color white = {255, 255, 255};

	char countdownText [4];
	sprintf(countdownText, "%d", countdown);

	SDL_Surface * countdownSurf = TTF_RenderText_Blended(font, countdownText, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, countdownSurf);

	setRectangle(&srcRect, 0, countdownSurf->h-(countdownSurf->h * fontFactor+1), countdownSurf->w, countdownSurf->h * fontFactor+1);
	setRectangle(&destRect, 4, countdownSurf->h * fontFactor+1, countdownSurf->w, countdownSurf->h * fontFactor+1);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(countdownSurf);


	//Finish turn/Cancel
	//BUG Seems to be blinking sometimes (misses some frames randomly)
	SDL_Surface * end = TTF_RenderText_Blended(font, message, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, end);

	setRectangle(&srcRect, 0, player->h-(player->h * fontFactor+1), end->w, end->h * fontFactor);
	setRectangle(&destRect, SCREEN_WIDTH-end->w, SCREEN_HEIGHT-(end->h*fontFactor)-4, end->w, end->h * fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(end);
	SDL_DestroyTexture(textTexture);

	SDL_Rect returnValue = destRect;


	//Gold indicator
	//Opening and closing font w/ different size would cause text to blink, so we need to change it in destRect

	char goldText [4];
	sprintf(goldText, "%d", game.players[game.currentPlayer].gold);

	SDL_Surface * gold = TTF_RenderText_Blended(font, goldText, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, gold);

	setRectangle(&srcRect, 0, gold->h-(gold->h * fontFactor), gold->w, gold->h * fontFactor);
	setRectangle(&destRect, 4, SCREEN_HEIGHT-(gold->h * fontFactor*1.5)-4, gold->w*1.5, gold->h * fontFactor*1.5);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	blitSprite(renderer, texture, 4, 23, destRect.x+destRect.w-(TILE_SIZE*1.2-5), destRect.y, TILE_SIZE*1.2);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(gold);


	//Wood indicator
	char woodText [4];
	sprintf(woodText, "%d", game.players[game.currentPlayer].wood);

	SDL_Surface * wood = TTF_RenderText_Blended(font, woodText, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, wood);

	setRectangle(&srcRect, 0, gold->h-(gold->h * fontFactor), wood->w, wood->h * fontFactor);
	setRectangle(&destRect, 4, destRect.y-(gold->h * fontFactor*1.5)-4, wood->w*1.5, wood->h * fontFactor*1.5);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	blitSprite(renderer, texture, 7, 23, destRect.x+destRect.w-(TILE_SIZE*1.2-5), destRect.y, TILE_SIZE*1.2);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(wood);

	TTF_CloseFont(font);

	return returnValue;	//Coords of 'End turn' button (needed for event)
}



int countdownUpdate(int * countdown, int * countdownSec, int quit, int * newEvent, game * game){
	*countdown -= REFRESH_PERIOD;
	if(*countdown/1000 < *countdownSec){	//Refresh display
		(*countdownSec)--;	// (╯°□°）╯︵ ┻━┻ Don't forget the parenthesis
		*newEvent = 1;

		if(*countdownSec < 0){	//Change player at end of countdown
			*countdown = TURN_TIME * 1000;
			*countdownSec = TURN_TIME;
			game->currentPlayer = (game->currentPlayer+1) % game->nPlayers;
			quit = 1;	//Back to mainHub for the next player
		}
	}

	return quit; //Indicates if the layer of Hud must be leaved (can be ignored for mainHud)
}



//Main HUD (default game state)
SDL_Rect mainDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown){
	SDL_Rect turnRect = basicDisplay(renderer, texture, game, camera, countdown, "END TURN");

	SDL_RenderPresent(renderer);
	return turnRect;
}



void mainHud(SDL_Renderer * renderer, SDL_Texture * texture, game game){
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
	SDL_Rect endButton = mainDisplay(renderer, texture, game, camera, countdownSec);

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
				busyReset(&game); //Resets units
			}

            switch(newEvent){
                case MENU:
                    quit = inGameMenu(renderer, texture);
					camera.leftClick = 0;	//Avoids moving camera if menu entered w click and left with Escp
                    break;

				case TILE_SELECTION:
					//Own unit
					tokenId = checkOwnUnit(game, selectedTile);
					if(tokenId < game.players[game.currentPlayer].nUnits){	//Cycles units
						switch(game.players[game.currentPlayer].units[tokenId].type){

							case PEASANT:
							quit = peasantHud(renderer, texture, &game, &camera, &countdown, &countdownSec, tokenId);
							break;

							case SOLDIER:
							quit = soldierHud(renderer, texture, &game, &camera, &countdown, &countdownSec, tokenId);
							break;
						}
					}
					else{	//Cycles buildingss
						tokenId = checkOwnBuilding(game, selectedTile);
						if(tokenId < game.players[game.currentPlayer].nBuildings){
								buildingHud(renderer, texture, &game, &camera, &countdown, &countdownSec, tokenId);

						}
					}
					//TODO Select also foreign Units
					/*else{

						int ownerId;
						int isUnit = 1;
						tokenId = checkForeignUnit(*game, selectedTile, &ownerId);
						if(ownerId > game->nPlayers)
							tokenId = checkForeignBuilding(*game, selectedTile, &ownerId);
							isUnit = 0;

						if(ownerId < game->nPlayers)
							quit = foreignHud(renderer, texture, &game, &camera, &countdown, &countdownSec, ownerId, tokenId, isUnit);
					}*/
            }
        }

		countdownUpdate(&countdown, &countdownSec, quit, &newEvent, &game);

        if(newEvent){  //Refresh display if a new event has occured
			mainDisplay(renderer, texture, game, camera, countdownSec);
			newEvent = 0;
		}
    }
}



//Peasant Hud
SDL_Rect peasantDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int peasantId){
	SDL_Rect cancelRect = basicDisplay(renderer, texture, game, camera, countdown, "CANCEL");

	//Highlighter
	coord highlighterCoords;
	highlighterCoords.x = camera.offset.x+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[peasantId].pos.x);
	highlighterCoords.y = camera.offset.y+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[peasantId].pos.y);
	blitSprite(renderer, texture, game.currentPlayer+4, 43, highlighterCoords.x, highlighterCoords.y, TILE_SIZE*camera.zoom);


	//Stats
	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", TILE_SIZE);
	SDL_Color white = {255, 255, 255};
	char text [6];

	SDL_Rect srcRect;
	SDL_Rect destRect;
	float fontFactor = 0.9;

	//Life
	blitSprite(renderer, texture, 7, 25, 0, SCREEN_HEIGHT/2-TILE_SIZE*1.5*3/2, TILE_SIZE*1.5);

	sprintf(text, "%dI%d", game.players[game.currentPlayer].units[peasantId].life, game.players[game.currentPlayer].units[peasantId].maxLife);

	SDL_Surface * life = TTF_RenderText_Blended(font, text, white);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, life);

	setRectangle(&srcRect, 0, life->h-(life->h*fontFactor), life->w, life->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-TILE_SIZE*3/2*1.33, life->w, life->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(life);
	SDL_DestroyTexture(textTexture);


	//Movements
	blitSprite(renderer, texture, 4, 16, 0, SCREEN_HEIGHT/2-TILE_SIZE*1.5/2, TILE_SIZE*1.5);

	sprintf(text, "%dI%d", game.players[game.currentPlayer].units[peasantId].movements, game.players[game.currentPlayer].units[peasantId].maxMovements);

	SDL_Surface * movements = TTF_RenderText_Blended(font, text, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, movements);

	setRectangle(&srcRect, 0, life->h-(movements->h*fontFactor), movements->w, movements->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-0.5*TILE_SIZE, movements->w, movements->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(movements);
	SDL_DestroyTexture(textTexture);


	//Attack
	blitSprite(renderer, texture, 5, 25, 0, SCREEN_HEIGHT/2+TILE_SIZE*1.5/2, TILE_SIZE*1.5);

	sprintf(text, "%d", game.players[game.currentPlayer].units[peasantId].attack);

	SDL_Surface * attack = TTF_RenderText_Blended(font, text, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, attack);

	setRectangle(&srcRect, 0, attack->h-(attack->h*fontFactor), attack->w, attack->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2+TILE_SIZE, attack->w, attack->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(attack);
	SDL_DestroyTexture(textTexture);


	TTF_CloseFont(font);

	if(!game.players[game.currentPlayer].units[peasantId].isBusy){
		//Move button
		blitSprite(renderer, texture, 4, 16, SCREEN_WIDTH/2-TILE_SIZE*2*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2-TILE_SIZE*2*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);


		//Attack button
		blitSprite(renderer, texture, 5, 25, SCREEN_WIDTH/2-TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2-TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);


		//Harvest button
		blitSprite(renderer, texture, 7, 14, SCREEN_WIDTH/2+TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2+TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);


		//Build button
		blitSprite(renderer, texture, 3, 2, SCREEN_WIDTH/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
	}


	//TODO Stats

	SDL_RenderPresent(renderer);
	return cancelRect;
}



int peasantHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int peasantId){
	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord target;
	coord * path;
	coord selectedTile;
	//int tokenId;
	//int ownerId;

	SDL_Rect cancelRect = peasantDisplay(renderer, texture, *game, *camera, *countdownSec, peasantId);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				camera->leftClick = 0;
				if(quitGame)
					quit = 1;
			}

			//Cancel button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>cancelRect.x && event.button.x<cancelRect.x+cancelRect.w
			&& event.button.y>cancelRect.y && event.button.y<cancelRect.y+cancelRect.y){
				quit = 1;
			}

			//Move button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2-TILE_SIZE*2*1.75 && event.button.x<SCREEN_WIDTH/2-TILE_SIZE*1.75
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 1, selectedTile, &target);

				int length = moveUnit(game, peasantId, target, &path);
				if(length){
					//TODO Animation
					game->players[game->currentPlayer].units[peasantId].pos.x = target.x;
					game->players[game->currentPlayer].units[peasantId].pos.y = target.y;

					target.x = 0;
					target.y = 0;
				}
			}
			//Attack Button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2-TILE_SIZE*1.75 && event.button.x<SCREEN_WIDTH/2
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				coord tokenPos = game->players[game->currentPlayer].units[peasantId].pos;
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 0, tokenPos, &target);

				attack(game, peasantId, target);
			}
		}

		//Countdown
		quit = countdownUpdate(countdown, countdownSec, quit, &newEvent, game);

		//Refresh
		if(newEvent){
			peasantDisplay(renderer, texture, *game, *camera, *countdownSec, peasantId);
		}
	}

	return quitGame;
}



//Soldier Hud
SDL_Rect soldierDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int soldierId){
	SDL_Rect cancelRect = basicDisplay(renderer, texture, game, camera, countdown, "CANCEL");

	//Highlighter
	coord highlighterCoords;
	highlighterCoords.x = camera.offset.x+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[soldierId].pos.x);
	highlighterCoords.y = camera.offset.y+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].units[soldierId].pos.y);
	blitSprite(renderer, texture, game.currentPlayer+4, 43, highlighterCoords.x, highlighterCoords.y, TILE_SIZE*camera.zoom);

	if(!game.players[game.currentPlayer].units[soldierId].isBusy){
		//Move button
		blitSprite(renderer, texture, 4, 16, SCREEN_WIDTH/2-TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2-TILE_SIZE*1.75, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);


		//Attack button
		blitSprite(renderer, texture, 5, 25, SCREEN_WIDTH/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		blitSprite(renderer, texture, game.currentPlayer+4, 43, SCREEN_WIDTH/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
	}


	//Stats
	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", TILE_SIZE);
	SDL_Color white = {255, 255, 255};
	char text [6];

	SDL_Rect srcRect;
	SDL_Rect destRect;
	float fontFactor = 0.9;

	//Life
	blitSprite(renderer, texture, 7, 25, 0, SCREEN_HEIGHT/2-TILE_SIZE*1.5*3/2, TILE_SIZE*1.5);

	sprintf(text, "%dI%d", game.players[game.currentPlayer].units[soldierId].life, game.players[game.currentPlayer].units[soldierId].maxLife);

	SDL_Surface * life = TTF_RenderText_Blended(font, text, white);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, life);

	setRectangle(&srcRect, 0, life->h-(life->h*fontFactor), life->w, life->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-TILE_SIZE*3/2*1.33, life->w, life->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(life);
	SDL_DestroyTexture(textTexture);


	//Movements
	blitSprite(renderer, texture, 4, 16, 0, SCREEN_HEIGHT/2-TILE_SIZE*1.5/2, TILE_SIZE*1.5);

	sprintf(text, "%dI%d", game.players[game.currentPlayer].units[soldierId].movements, game.players[game.currentPlayer].units[soldierId].maxMovements);

	SDL_Surface * movements = TTF_RenderText_Blended(font, text, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, movements);

	setRectangle(&srcRect, 0, life->h-(movements->h*fontFactor), movements->w, movements->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-0.5*TILE_SIZE, movements->w, movements->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(movements);
	SDL_DestroyTexture(textTexture);


	//Attack
	blitSprite(renderer, texture, 5, 25, 0, SCREEN_HEIGHT/2+TILE_SIZE*1.5/2, TILE_SIZE*1.5);

	sprintf(text, "%d", game.players[game.currentPlayer].units[soldierId].attack);

	SDL_Surface * attack = TTF_RenderText_Blended(font, text, white);
	textTexture = SDL_CreateTextureFromSurface(renderer, attack);

	setRectangle(&srcRect, 0, attack->h-(attack->h*fontFactor), attack->w, attack->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2+TILE_SIZE, attack->w, attack->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(attack);
	SDL_DestroyTexture(textTexture);


	TTF_CloseFont(font);

	SDL_RenderPresent(renderer);
	return cancelRect;
}



int soldierHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int soldierId){
	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	coord target;
	coord * path;

	SDL_Rect cancelRect = soldierDisplay(renderer, texture, *game, *camera, *countdownSec, soldierId);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				camera->leftClick = 0;
				if(quitGame)
					quit = 1;
			}

			//Cancel button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>cancelRect.x && event.button.x<cancelRect.x+cancelRect.w
			&& event.button.y>cancelRect.y && event.button.y<cancelRect.y+cancelRect.y){
				quit = 1;
			}

			//Move button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2-TILE_SIZE*1.75 && event.button.x<SCREEN_WIDTH/2
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 1, selectedTile, &target);

				int length = moveUnit(game, soldierId, target, &path);
				if(length){
					//TODO Animation
					game->players[game->currentPlayer].units[soldierId].pos.x = target.x;
					game->players[game->currentPlayer].units[soldierId].pos.y = target.y;

					target.x = 0;
					target.y = 0;
				}
			}
			//Attack button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2 && event.button.x<SCREEN_WIDTH/2+TILE_SIZE*1.75
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				coord tokenPos = game->players[game->currentPlayer].units[soldierId].pos;
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 0, tokenPos, &target);

				attack(game, soldierId, target);
			}
		}

		//Countdown
		quit = countdownUpdate(countdown, countdownSec, quit, &newEvent, game);

		//Refresh
		if(newEvent){
			soldierDisplay(renderer, texture, *game, *camera, *countdownSec, soldierId);
		}
	}

	return quitGame;
}



//Building Hud
SDL_Rect buildingDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int buildingId){
	SDL_Rect cancelRect = basicDisplay(renderer, texture, game, camera, countdown, "CANCEL");

	//Highlighter
	coord highlighterCoords;
	highlighterCoords.x = camera.offset.x+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].buildings[buildingId].pos.x);
	highlighterCoords.y = camera.offset.y+TILE_SIZE*camera.zoom*(game.players[game.currentPlayer].buildings[buildingId].pos.y);
	blitSprite(renderer, texture, game.currentPlayer+4, 43, highlighterCoords.x, highlighterCoords.y, TILE_SIZE*camera.zoom);

	if(!game.players[game.currentPlayer].buildings[buildingId].isBusy){
		//Create Unit Button
		switch(game.players[game.currentPlayer].buildings[buildingId].type){
			case CITY:
				blitSprite(renderer, texture, 7, 14, SCREEN_WIDTH/2-TILE_SIZE*1.75/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
				break;

			case BARRACK:
				blitSprite(renderer, texture, 7, 17, SCREEN_WIDTH/2-TILE_SIZE*1.75/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);
		}
		blitSprite(renderer, texture, 4+game.currentPlayer, 43, SCREEN_WIDTH/2-TILE_SIZE*1.75/2, SCREEN_HEIGHT-TILE_SIZE*1.75, TILE_SIZE*1.75);

	}


	//Stats
	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", TILE_SIZE);
	SDL_Color white = {255, 255, 255};
	char text [6];

	SDL_Rect srcRect;
	SDL_Rect destRect;
	float fontFactor = 0.9;

	//Life
	blitSprite(renderer, texture, 7, 25, 0, SCREEN_HEIGHT/2-TILE_SIZE*0.75, TILE_SIZE*1.5);

	sprintf(text, "%dI%d", game.players[game.currentPlayer].buildings[buildingId].life, game.players[game.currentPlayer].buildings[buildingId].maxLife);

	SDL_Surface * life = TTF_RenderText_Blended(font, text, white);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, life);

	setRectangle(&srcRect, 0, life->h-(life->h*fontFactor), life->w, life->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-0.5*TILE_SIZE, life->w, life->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(life);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);


	SDL_RenderPresent(renderer);
	return cancelRect;
}



int buildingHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int buildingId){
	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	//int tokenId;
	//int ownerId;

	SDL_Rect cancelRect = buildingDisplay(renderer, texture, *game, *camera, *countdownSec, buildingId);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				camera->leftClick = 0;
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
		quit = countdownUpdate(countdown, countdownSec, quit, &newEvent, game);

		//Refresh
		if(newEvent){
			buildingDisplay(renderer, texture, *game, *camera, *countdownSec, buildingId);
		}
	}

	return quitGame;
}



//Target selection HUD
SDL_Rect targetDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int isMovement, coord pos){
	SDL_Rect cancelRect = basicDisplay(renderer, texture, game, camera, countdown, "CANCEL");

	SDL_Rect srcRect;
	SDL_Rect destRect;


	//Upper message
	SDL_Color white = {255, 255, 255};
	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", SCREEN_HEIGHT/28);
	float fontFactor = 0.9;

	SDL_Surface * messageSurf;
	if(isMovement) {
		messageSurf = TTF_RenderText_Blended(font, "SELECT DESTINATION", white);
	}
	else{
		messageSurf = TTF_RenderText_Blended(font, "SELECT TARGET", white);
	}

	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, messageSurf);

	setRectangle(&srcRect, 0, messageSurf->h-(messageSurf->h * fontFactor+1), messageSurf->w, messageSurf->h * fontFactor+1);
	setRectangle(&destRect, SCREEN_WIDTH/2-messageSurf->w/2, SCREEN_HEIGHT/20, messageSurf->w, messageSurf->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(messageSurf);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);


	//Highlighters
	if(!isMovement){
		blitSprite(renderer, texture, 4+game.currentPlayer, 43, camera.offset.x+(pos.x+1)*TILE_SIZE*camera.zoom, camera.offset.y+(pos.y)*TILE_SIZE*camera.zoom, TILE_SIZE*camera.zoom);
		blitSprite(renderer, texture, 4+game.currentPlayer, 43, camera.offset.x+(pos.x-1)*TILE_SIZE*camera.zoom, camera.offset.y+(pos.y)*TILE_SIZE*camera.zoom, TILE_SIZE*camera.zoom);
		blitSprite(renderer, texture, 4+game.currentPlayer, 43, camera.offset.x+(pos.x)*TILE_SIZE*camera.zoom, camera.offset.y+(pos.y+1)*TILE_SIZE*camera.zoom, TILE_SIZE*camera.zoom);
		blitSprite(renderer, texture, 4+game.currentPlayer, 43, camera.offset.x+(pos.x)*TILE_SIZE*camera.zoom, camera.offset.y+(pos.y-1)*TILE_SIZE*camera.zoom, TILE_SIZE*camera.zoom);
	}


	SDL_RenderPresent(renderer);
	return cancelRect;
}


int targetHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int isMovement, coord pos, coord * target){
	//General selection Hud used to retrieve clicked tile

	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	//int tokenId;
	//int ownerId;

	SDL_Rect cancelRect = targetDisplay(renderer, texture, *game, *camera, *countdownSec, isMovement, pos);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				camera->leftClick = 0;
				if(quitGame)
					quit = 1;
			}

			//Cancel button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>cancelRect.x && event.button.x<cancelRect.x+cancelRect.w
			&& event.button.y>cancelRect.y && event.button.y<cancelRect.y+cancelRect.y){
				quit = 1;
			}

			else if(newEvent == TILE_SELECTION){
				*target = selectedTile;
				quit = 1;
			}
		}

		//Countdown
		quit = countdownUpdate(countdown, countdownSec, quit, &newEvent, game);

		//Refresh
		if(newEvent){
			targetDisplay(renderer, texture, *game, *camera, *countdownSec, isMovement, pos);
		}
	}

	return quitGame;
}



//Foreign Token Hud
SDL_Rect foreignDisplay(SDL_Renderer * renderer, SDL_Texture * texture, game game, view camera, int countdown, int ownerId, int tokenId, int isUnit){
	SDL_Rect cancelRect = basicDisplay(renderer, texture, game, camera, countdown, "CANCEL");

	coord pos;

	if(isUnit){
		pos.x =	game.players[ownerId].units[tokenId].pos.x;
		pos.y =	game.players[ownerId].units[tokenId].pos.y;
	}
	else{
		pos.x =	game.players[ownerId].buildings[tokenId].pos.x;
		pos.y =	game.players[ownerId].buildings[tokenId].pos.y;
	}

	//Highlighter
	coord highlighterCoords;
	highlighterCoords.x = camera.offset.x+TILE_SIZE*camera.zoom*(pos.x);
	highlighterCoords.y = camera.offset.y+TILE_SIZE*camera.zoom*(pos.y);
	blitSprite(renderer, texture, game.currentPlayer+4, 43, highlighterCoords.x, highlighterCoords.y, TILE_SIZE*camera.zoom);


	//Stats
	TTF_Font * font = TTF_OpenFont("resources/8bit.ttf", TILE_SIZE);
	SDL_Color white = {255, 255, 255};
	char text [6];

	SDL_Rect srcRect;
	SDL_Rect destRect;
	float fontFactor = 0.9;

	//Life
	blitSprite(renderer, texture, 7, 25, 0, SCREEN_HEIGHT/2-TILE_SIZE*0.75, TILE_SIZE*1.5);

	if(isUnit)
		sprintf(text, "%dI%d", game.players[ownerId].units[tokenId].life, game.players[ownerId].units[tokenId].maxLife);
	else
		sprintf(text, "%dI%d", game.players[ownerId].buildings[tokenId].life, game.players[ownerId].buildings[tokenId].maxLife);

	SDL_Surface * life = TTF_RenderText_Blended(font, text, white);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, life);

	setRectangle(&srcRect, 0, life->h-(life->h*fontFactor), life->w, life->h*fontFactor);
	setRectangle(&destRect, TILE_SIZE*1.5, SCREEN_HEIGHT/2-0.5*TILE_SIZE, life->w, life->h*fontFactor);
	SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

	SDL_FreeSurface(life);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);


	SDL_RenderPresent(renderer);
	return cancelRect;
}


//Still trying to figure out what I did there
/*
int foreignHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int ownerId, int tokenId, int isUnit){
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	//int tokenId;
	//int ownerId;

	SDL_Rect cancelRect = buildingDisplay(renderer, texture, *game, *camera, *countdownSec, buildingId);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				camera->leftClick = 0;
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
		quit = countdownUpdate(countdown, countdownSec, quit, &newEvent, game);

		//Refresh
		if(newEvent){
			buildingDisplay(renderer, texture, *game, *camera, *countdownSec, buildingId);
		}
	}

	return quitGame;
}
*/

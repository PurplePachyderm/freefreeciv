#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/hud.h"
#include "../include/display/hud_display.h"
#include "../include/display/display.h"
#include "../include/display/menu.h"

#include "../include/coord.h"
#include "../include/game/game.h"
#include "../include/game/units_actions.h"


//Basic functions (reused in several Huds)
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
					else if(checkOwnBuilding(game, selectedTile) < game.players[game.currentPlayer].nBuildings){	//Cycles buildingss
						tokenId = checkOwnBuilding(game, selectedTile);
						if(tokenId < game.players[game.currentPlayer].nBuildings){
								quit = buildingHud(renderer, texture, &game, &camera, &countdown, &countdownSec, tokenId);

						}
					}
					else if(checkOwnUnit(game, selectedTile) < game.players[game.currentPlayer].nUnits){	//Cycles buildingss
						tokenId = checkOwnBuilding(game, selectedTile);
						if(tokenId < game.players[game.currentPlayer].nBuildings){
								quit = buildingHud(renderer, texture, &game, &camera, &countdown, &countdownSec, tokenId);

						}
					}
					//TODO Select also foreign Units
					else{

						int ownerId;
						int isUnit = 1;
						tokenId = checkForeignUnit(game, selectedTile, &ownerId);
						if(ownerId > game.nPlayers){
							tokenId = checkForeignBuilding(game, selectedTile, &ownerId);
							isUnit = 0;
						}


						if(ownerId < game.nPlayers){
							quit = foreignHud(renderer, texture, &game, &camera, &countdown, &countdownSec, ownerId, tokenId, isUnit);
						}
					}
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
				quit = quitGame;

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
				quit = quitGame;

				attack(game, peasantId, target);
			}

			//Barrack Creation Button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2 && event.button.x<SCREEN_WIDTH/2+TILE_SIZE*1.75
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				coord tokenPos = game->players[game->currentPlayer].units[peasantId].pos;
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 0, tokenPos, &target);
				quit = quitGame;

				createBarrack(game, target, peasantId);
			}

			//Collect Button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2+TILE_SIZE*1.75 && event.button.x<SCREEN_WIDTH/2+TILE_SIZE*2*1.75
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				coord tokenPos = game->players[game->currentPlayer].units[peasantId].pos;
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 0, tokenPos, &target);
				quit = quitGame;

				collect(game, peasantId, target);
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
				quit = quitGame;
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
				quit = quitGame;

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
				quit = quitGame;

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
int buildingHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int buildingId){
	SDL_Event event;
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;

	coord selectedTile;
	coord target;
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
				quit = quitGame;
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

			//Create unit button
			//Attack Button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x>SCREEN_WIDTH/2-TILE_SIZE*1.75/2 && event.button.x<SCREEN_WIDTH/2+TILE_SIZE*1.75/2
			&& event.button.y>SCREEN_HEIGHT-TILE_SIZE*1.75 && event.button.y<SCREEN_HEIGHT){
				coord tokenPos = game->players[game->currentPlayer].buildings[buildingId].pos;
				quitGame = targetHud(renderer, texture, game, camera, countdown, countdownSec, 0, tokenPos, &target);
				quit = quitGame;

				switch(game->players[game->currentPlayer].buildings[buildingId].type){
					case CITY:
						createPeasant(game, target, buildingId);
						break;

					case BARRACK:
						createSoldier(game, target, buildingId);
						break;
				}
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
//XXX Not finished (Needs dedicated display function)

int foreignHud(SDL_Renderer * renderer, SDL_Texture * texture, game * game, view * camera, int * countdown, int * countdownSec, int ownerId, int tokenId, int isUnit){
	int quit = 0;
	int quitGame = 0;	//Return value (quits the entire game)
	int newEvent = 0;
	SDL_Event event;

	coord selectedTile;
	//int tokenId;
	//int ownerId;


	SDL_Rect cancelRect = foreignDisplay(renderer, texture, *game, *camera, *countdownSec, ownerId, tokenId, isUnit);

	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		//Events
		while(SDL_PollEvent(&event)){
			newEvent = events(event, camera, *game, &selectedTile);

			//Menu
			if(newEvent == MENU){
				quitGame = inGameMenu(renderer, texture);
				quit = quitGame;
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
			foreignDisplay(renderer, texture, *game, *camera, *countdownSec, ownerId, tokenId, isUnit);
		}
	}

	return quitGame;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/menu.h"
#include "../include/display/display.h"
#include "../include/display/hud.h"
#include "../include/game/map.h"


	//***Main menu***
void mainMenu(SDL_Renderer * renderer, SDL_Texture * texture){
	SDL_Event event;
	int quit = 0;

	//Font size and surface height are different, but we need to locate the actual text for hitboxes
	float fontFactor = 0.655;

	//Is 1 when we come back to this menu to refresh display only when needed
	int refresh = 1;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	SDL_Surface * background = NULL;
	SDL_Surface * title = NULL;
	SDL_Surface * localPlay = NULL;
	SDL_Surface * load = NULL;
	SDL_Surface * multiplayer = NULL;
	SDL_Surface * sQuit = NULL;


	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		if(refresh){

			SDL_FreeSurface(background);
			SDL_FreeSurface(title);
			SDL_FreeSurface(localPlay);
			SDL_FreeSurface(load);
			SDL_FreeSurface(multiplayer);
			SDL_FreeSurface(sQuit);

			//Background
			setRectangle(&srcRect, 0, 0, 3840, 2160); //Dim of background
			setRectangle(&destRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			background = IMG_Load("resources/menu.png");
			SDL_Texture * backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
			SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);
			SDL_DestroyTexture(backgroundTexture);



			//Title
			TTF_Font * font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/8);
			SDL_Color color = {255, 237, 43};

			title = TTF_RenderText_Blended(font, "freefreeciv", color);
			SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, title);

			setRectangle(&srcRect, 0, title->h - (title->h*fontFactor+1), title->w, title->h * fontFactor);
			setRectangle(&destRect, SCREEN_WIDTH/2 - title->w/2, 3*SCREEN_HEIGHT/64, title->w, title->h * fontFactor);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);
			TTF_CloseFont(font);


			//Local play
			font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/16);

			localPlay = TTF_RenderText_Blended(font, "local play", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, localPlay);

			setRectangle(&srcRect, 0, localPlay->h - (localPlay->h*fontFactor+1), localPlay->w, localPlay->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - localPlay->w/2, 3*SCREEN_HEIGHT/8-((localPlay->h*fontFactor+1)/2), localPlay->w, localPlay->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Load
			load = TTF_RenderText_Blended(font, "load", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, load);

			setRectangle(&srcRect, 0, load->h - (load->h*fontFactor+1), load->w, load->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - load->w/2, 2*SCREEN_HEIGHT/4-((load->h*fontFactor+1)/2), load->w, load->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Multiplayer
			multiplayer = TTF_RenderText_Blended(font, "multiplayer", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, multiplayer);

			setRectangle(&srcRect, 0, multiplayer->h - (multiplayer->h*fontFactor+1), multiplayer->w, multiplayer->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - multiplayer->w/2, 5*SCREEN_HEIGHT/8-((multiplayer->h*fontFactor+1)/2), multiplayer->w, multiplayer->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);



			//Quit
			sQuit = TTF_RenderText_Blended(font, "quit", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, sQuit);

			setRectangle(&srcRect, 0, sQuit->h - (sQuit->h*fontFactor+1), sQuit->w, sQuit->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - sQuit->w/2, 6*SCREEN_HEIGHT/8-((sQuit->h*fontFactor+1)/2), sQuit->w, sQuit->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);



			SDL_RenderPresent(renderer);
			TTF_CloseFont(font);

			refresh = 0;
		}


		//Events management
		while(SDL_PollEvent(&event)){
			//New local game
			if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - localPlay->w/2 && event.button.x <= SCREEN_WIDTH/2 + localPlay->w/2
			&& event.button.y >= 3*SCREEN_HEIGHT/8-((localPlay->h*fontFactor+1)/2) && event.button.y <= 3*SCREEN_HEIGHT/8-((localPlay->h*fontFactor+1)/2)+localPlay->h * fontFactor + 1){
				quit = newGameMenu(renderer, texture);
				refresh = 1;
			}


			//Quit game ("Quit" button)
			if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - sQuit->w/2 && event.button.x <= SCREEN_WIDTH/2 + sQuit->w/2
			&& event.button.y >= 6*SCREEN_HEIGHT/8-((sQuit->h*fontFactor+1)/2) && event.button.y <= 6*SCREEN_HEIGHT/8-((sQuit->h*fontFactor+1)/2)+sQuit->h * fontFactor + 1){
				quit = 1;
			}
		}
	}

	SDL_FreeSurface(background);
	SDL_FreeSurface(title);
	SDL_FreeSurface(localPlay);
	SDL_FreeSurface(load);
	SDL_FreeSurface(multiplayer);
	SDL_FreeSurface(sQuit);

}



	//***New game menu***
int newGameMenu(SDL_Renderer * renderer, SDL_Texture * texture){

	SDL_Event event;
	int quit = 0;
	int quitGame = 0;

	//Font size and surface height are different, but we need to locate the actual text for hitboxes
	float fontFactor = 0.655;

	//Is 1 when we come back to this menu to refresh display only when needed
	int refresh = 1;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	SDL_Surface * background = NULL;
	SDL_Surface * title = NULL;
	SDL_Surface * slot1 = NULL;
	SDL_Surface * slot2 = NULL;
	SDL_Surface * slot3 = NULL;
	SDL_Surface * slot4 = NULL;
	SDL_Surface * start = NULL;


	struct game game;
	game.nPlayers = 2;

	int slots [4];
	slots [0] = HUMAN;
	slots [1] = AI_SLOT;
	slots [2] = EMPTY;
	slots [3] = EMPTY;

	//Used for the naming of slot players ("Player 1", "AI 3", etc...)
	int currentPlayer = 1;
	int currentAI = 1;

	//Contains the names of the 4 slots
	char slotName [17];


	while(!quit){
        SDL_Delay(REFRESH_PERIOD);

		if(refresh == 1){

			//Frees previous surfaces before allocating the new ones
			SDL_FreeSurface(background);
			SDL_FreeSurface(title);
			SDL_FreeSurface(slot1);
			SDL_FreeSurface(slot2);
			SDL_FreeSurface(slot3);
			SDL_FreeSurface(slot4);
			SDL_FreeSurface(start);

			//Background
			setRectangle(&srcRect, 0, 0, 3840, 2160); //Dim of background
			setRectangle(&destRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			background = IMG_Load("resources/menu.png");
			SDL_Texture * backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
			SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);

			SDL_DestroyTexture(backgroundTexture);


			//Title
			TTF_Font * font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/8);
			SDL_Color color = {255, 237, 43};

			title = TTF_RenderText_Blended(font, "new game", color);
			SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, title);

			setRectangle(&srcRect, 0, title->h - (title->h*fontFactor+1), title->w, title->h * fontFactor);
			setRectangle(&destRect, SCREEN_WIDTH/2 - title->w/2, 3*SCREEN_HEIGHT/64, title->w, title->h * fontFactor);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);
			TTF_CloseFont(font);


			//Slot 1
			font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/16);

			if(slots[0] == HUMAN){
				sprintf(slotName, "slot 1: player %d", currentPlayer);
				currentPlayer++;
			}
			else if(slots[0] == AI_SLOT){
				sprintf(slotName, "slot 1: ai %d", currentAI);
				currentAI++;
			}
			else{
				sprintf(slotName, "slot 1: empty");
			}

			slot1 = TTF_RenderText_Blended(font, slotName, color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot1);

			setRectangle(&srcRect, 0, slot1->h - (slot1->h*fontFactor+1), slot1->w, slot1->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot1->w/2, 2*SCREEN_HEIGHT/8-((slot1->h*fontFactor+1)/2), slot1->w, slot1->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 2
			if(slots[1] == HUMAN){
				sprintf(slotName, "slot 2: player %d", currentPlayer);
				currentPlayer++;
			}
			else if(slots[1] == AI_SLOT){
				sprintf(slotName, "slot 2: ai %d", currentAI);
				currentAI++;
			}
			else{
				sprintf(slotName, "slot 2: empty");
			}

			slot2 = TTF_RenderText_Blended(font, slotName, color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot2);

			setRectangle(&srcRect, 0, slot2->h - (slot2->h*fontFactor+1), slot2->w, slot2->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot2->w/2, 3*SCREEN_HEIGHT/8-((slot2->h*fontFactor+1)/2), slot2->w, slot2->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 3
			if(slots[2] == HUMAN){
				sprintf(slotName, "slot 3: player %d", currentPlayer);
				currentPlayer++;
			}
			else if(slots[2] == AI_SLOT){
				sprintf(slotName, "slot 3: ai %d", currentAI);
				currentAI++;
			}
			else{
				sprintf(slotName, "slot 3: empty");
			}

			slot3 = TTF_RenderText_Blended(font, slotName, color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot3);

			setRectangle(&srcRect, 0, slot3->h - (slot3->h*fontFactor+1), slot3->w,slot3->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot3->w/2, SCREEN_HEIGHT/2-((slot3->h*fontFactor+1)/2), slot3->w, slot3->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 4
			if(slots[3] == HUMAN){
				sprintf(slotName, "slot 4: player %d", currentPlayer);
				currentPlayer++;
			}
			else if(slots[3] == AI_SLOT){
				sprintf(slotName, "slot 4: ai %d", currentAI);
				currentAI++;
			}
			else{
				sprintf(slotName, "slot 4: empty");
			}

			slot4 = TTF_RenderText_Blended(font, slotName, color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot4);

			setRectangle(&srcRect, 0, slot4->h - (slot4->h*fontFactor+1), slot4->w, slot4->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot4->w/2, 5*SCREEN_HEIGHT/8-((slot4->h*fontFactor+1)/2), slot4->w, slot4->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Start button
			start = TTF_RenderText_Blended(font, "start", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, start);

			setRectangle(&srcRect, 0, start->h - (start->h*fontFactor+1), start->w, start->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - start->w/2, 7*SCREEN_HEIGHT/8-((start->h*fontFactor+1)/2), start->w, start->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			SDL_RenderPresent(renderer);
			TTF_CloseFont(font);

			//Blocks refreshing
			refresh = 0;

			//Resets naming
			currentPlayer = 1;
			currentAI = 1;
		}

		//Events management
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                quit = 1;
            }

			//Click on slot 1
			if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - slot1->w/2 && event.button.x <= SCREEN_WIDTH/2 + slot1->w/2
			&& event.button.y >= 2*SCREEN_HEIGHT/8-((slot1->h*fontFactor+1)/2) && event.button.y <= 2*SCREEN_HEIGHT/8-((slot1->h*fontFactor+1)/2)+slot1->h * fontFactor + 1){
				slots[0] = cycleSlot(slots, 0, 1);
				refresh = 1;
			}

			//Click on slot 2
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - slot2->w/2 && event.button.x <= SCREEN_WIDTH/2 + slot2->w/2
			&& event.button.y >= 3*SCREEN_HEIGHT/8-((slot2->h*fontFactor+1)/2) && event.button.y <= 3*SCREEN_HEIGHT/8-((slot2->h*fontFactor+1)/2)+slot2->h * fontFactor + 1){
				slots[1] = cycleSlot(slots, 1, 1);
				refresh = 1;
			}

			//Click on slot 3
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - slot3->w/2 && event.button.x <= SCREEN_WIDTH/2 + slot3->w/2
			&& event.button.y >= 4*SCREEN_HEIGHT/8-((slot3->h*fontFactor+1)/2) && event.button.y <= 4*SCREEN_HEIGHT/8-((slot3->h*fontFactor+1)/2)+slot3->h * fontFactor + 1){
				slots[2] = cycleSlot(slots, 2, 1);
				refresh = 1;
			}

			//Click on slot 4
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - slot4->w/2 && event.button.x <= SCREEN_WIDTH/2 + slot4->w/2
			&& event.button.y >= 5*SCREEN_HEIGHT/8-((slot4->h*fontFactor+1)/2) && event.button.y <= 5*SCREEN_HEIGHT/8-((slot4->h*fontFactor+1)/2)+slot4->h * fontFactor + 1){
				slots[3] = cycleSlot(slots, 3, 1);
				refresh = 1;
			}

			//Click Start button
			else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT
			&& event.button.x >= SCREEN_WIDTH/2 - start->w/2 && event.button.x <= SCREEN_WIDTH/2 + start->w/2
			&& event.button.y >= 7*SCREEN_HEIGHT/8-((start->h*fontFactor+1)/2) && event.button.y <= 7*SCREEN_HEIGHT/8-((start->h*fontFactor+1)/2)+start->h * fontFactor + 1){
				int nPlayers = 0;

				//Get the number of players
				for(int i=0; i<4; i++){
					if(slots[i] != EMPTY)
						nPlayers++;
				}

				int * isAIControlled;
				isAIControlled = (int*) malloc(sizeof(int)*nPlayers);

				int currentSlot = 0;
				for(int i=0; i<4; i++){
					if(slots[i] == HUMAN){
						isAIControlled[currentSlot] = 0;
						currentSlot++;
					}

					else if(slots[i] == AI_SLOT){
						isAIControlled[currentSlot] = 1;
						currentSlot++;
					}
				}

				genGame(&game, nPlayers, isAIControlled);

				mainHud(renderer, texture, game);
				quit = 1;
			}
        }

    }

	SDL_FreeSurface(background);
	SDL_FreeSurface(title);
	SDL_FreeSurface(slot1);
	SDL_FreeSurface(slot2);
	SDL_FreeSurface(slot3);
	SDL_FreeSurface(slot4);
	SDL_FreeSurface(start);

	return quitGame;
}



int cycleSlot(int * slots, int slotId, int firstIteration){
	//firstIteration allows to test the 2 other slot possibilities instead
	//of just the next one by running the function again

	int returnValue;
	int newValue;

	//Calculates the value that the slot is supposed to take (see macros in menu.h)
	if(firstIteration)
		newValue = (slots[slotId] + 1) % 3;
	else
		//Second possible value if it's not thee fist try
		newValue = (slots[slotId] + 2) % 3;

	int hasHuman = 0;
	int nPlayers = 0;

	int testedSlot;

	for(int i=0; i<4; i++){
		//Allows to test the new configuration
		if(i == slotId)
			testedSlot = newValue;
		else
			testedSlot = slots[i];

		if(testedSlot == HUMAN)
			hasHuman = 1;

		if(testedSlot != EMPTY){
			nPlayers++;
		}
	}

	//We need at least 2 players with at least 1 human
	if(nPlayers >= 2 && hasHuman){
		returnValue = newValue;	//We allow the switch
	}

	else{
		if(firstIteration)
			//We try the other value
			returnValue = cycleSlot(slots, slotId, 0);
		else
			//We leave the value unchanged
			returnValue = slots[slotId];	//The value stays the same
	}

	return returnValue;
}



//***Menu HUD (Quit game, load & save)***
int inGameMenu(SDL_Renderer * renderer){
    SDL_Event event;
    int quit = 0;
    int exitGame = 0; //Return value

    //Font size and surface height are different, but we need to locate the actual text for hitboxes
    float fontFactor = 0.655;


    //Background
	//BUG Seems to be freed incorrectly sometimes (abnormal memory consumption but no crash)
    SDL_Rect srcRect;
    setRectangle(&srcRect, 0, 0, 3840, 2160); //Dim of background

    SDL_Rect destRect;
    setRectangle(&destRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface * background = IMG_Load("resources/menu.png");
    SDL_Texture * backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);

    SDL_DestroyTexture(backgroundTexture);

    //Title
    TTF_Font * font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/8);
    SDL_Color color = {255, 237, 43};

    SDL_Surface * title= TTF_RenderText_Blended(font, "freefreeciv", color);
    SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, title);

    setRectangle(&srcRect, 0, title->h - (title->h*fontFactor+1), title->w, title->h * fontFactor);
    setRectangle(&destRect, SCREEN_WIDTH/2 - title->w/2, 3*SCREEN_HEIGHT/64, title->w, title->h * fontFactor);
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

    SDL_RenderPresent(renderer);
	TTF_CloseFont(font);

    //Loop (display doesn't change anymore)
    while(!quit){
        SDL_Delay(REFRESH_PERIOD);

		//Events management
        while(SDL_PollEvent(&event)){
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
				exitGame = QUIT_GAME;
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

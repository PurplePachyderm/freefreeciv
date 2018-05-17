#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../include/display/menu.h"
#include "../include/display/display.h"
#include "../include/display/hud.h"


//Main menu
void mainMenu(SDL_Renderer * renderer, SDL_Texture * texture){
	SDL_Event event;
	int quit = 0;

	//Font size and surface height are different, but we need to locate the actual text for hitboxes
	float fontFactor = 0.655;

	//Is 1 when we come back to this menu to refresh display only when needed
	int refresh = 1;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	SDL_Surface * background;
	SDL_Surface * title;
	SDL_Surface * localPlay;
	SDL_Surface * load;
	SDL_Surface * multiplayer;
	SDL_Surface * sQuit;

	//Event loop
	while(!quit){
		SDL_Delay(REFRESH_PERIOD);

		if(refresh){
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
			font = TTF_OpenFont("resources/starjedi.ttf", SCREEN_HEIGHT/16);

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



//New game menu
int newGameMenu(SDL_Renderer * renderer, SDL_Texture * texture){
	//BUG Some memory is not freed correctly

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

			slot1 = TTF_RenderText_Blended(font, "slot 1", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot1);

			setRectangle(&srcRect, 0, slot1->h - (slot1->h*fontFactor+1), slot1->w, slot1->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot1->w/2, 3*SCREEN_HEIGHT/8-((slot1->h*fontFactor+1)/2), slot1->w, slot1->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 2
			slot2 = TTF_RenderText_Blended(font, "slot 2", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot2);

			setRectangle(&srcRect, 0, slot2->h - (slot2->h*fontFactor+1), slot2->w, slot2->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot2->w/2, 2*SCREEN_HEIGHT/4-((slot2->h*fontFactor+1)/2), slot2->w, slot2->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 3
			slot3 = TTF_RenderText_Blended(font, "slot 3", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot3);

			setRectangle(&srcRect, 0, slot3->h - (slot3->h*fontFactor+1), slot3->w,slot3->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot3->w/2, 5*SCREEN_HEIGHT/8-((slot3->h*fontFactor+1)/2), slot3->w, slot3->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			//Slot 4
			slot4 = TTF_RenderText_Blended(font, "slot 4", color);
			textTexture = SDL_CreateTextureFromSurface(renderer, slot4);

			setRectangle(&srcRect, 0, slot4->h - (slot4->h*fontFactor+1), slot4->w, slot4->h * fontFactor + 1);
			setRectangle(&destRect, SCREEN_WIDTH/2 - slot4->w/2, 6*SCREEN_HEIGHT/8-((slot4->h*fontFactor+1)/2), slot4->w, slot4->h * fontFactor + 1);
			SDL_RenderCopy(renderer, textTexture, &srcRect, &destRect);

			SDL_DestroyTexture(textTexture);


			SDL_RenderPresent(renderer);
			TTF_CloseFont(font);

			refresh = 0;
		}

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
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

	return quitGame;
}


//Menu HUD (Quit game, load & save)
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

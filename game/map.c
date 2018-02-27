#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../include/game/map.h"
#include "../include/game/game.h"
#include "../include/game/units_actions.h"

//XXX Needs testing

void genSpawns(game * game){
	int dist;
	int validCoords = 0;

	do{
		//Generation
		for(int i=0; i<game->nPlayers; i++){
			game->players[i].buildings[0].pos.x = (rand()%MAP_SIZE)+1;
			game->players[i].buildings[0].pos.y = (rand()%MAP_SIZE)+1;

			int peasantPos = rand()%4;	//Randomizes starting pos of peasant

			switch(peasantPos){
				case 0:	//Up
					game->players[i].units[0].pos.x = game->players[i].buildings[0].pos.x - 1;
					game->players[i].units[0].pos.y = game->players[i].buildings[0].pos.y;
					break;

				case 1:	//Right
					game->players[i].units[0].pos.x = game->players[i].buildings[0].pos.x;
					game->players[i].units[0].pos.y = game->players[i].buildings[0].pos.y + 1;
					break;

				case 2:	//Bottom
					game->players[i].units[0].pos.x = game->players[i].buildings[0].pos.x + 1;
					game->players[i].units[0].pos.y = game->players[i].buildings[0].pos.y;
					break;

				case 3:	//Left
					game->players[i].units[0].pos.x = game->players[i].buildings[0].pos.x;
					game->players[i].units[0].pos.y = game->players[i].buildings[0].pos.y - 1;
					break;
			}
		}

		//Verification
		validCoords = 1;
		for(int i=0; i<game->nPlayers; i++){
			for(int j=0; j<i; j++){

				int distX = abs(game->players[i].buildings[0].pos.x - game->players[j].buildings[0].pos.x);
				int distY = abs(game->players[i].buildings[0].pos.y - game->players[j].buildings[0].pos.y);
				dist = distX + distY;

				if(dist <= MIN_DIST){
					validCoords = 0;
					break;
				}

			}
			if(validCoords)
				validCoords = !checkMap(*game, game->players[i].units[0].pos);

			if(!validCoords)
				break;
		}

    }while(!validCoords);
}



void genResources(game * game){
	game->map.nResources = (N_RESOURCES + game->nPlayers - 2)*2;	//N_RESOURCES is the number for each type
	game->map.resources = (resource*)malloc(sizeof(resource)*game->map.nResources);

	int validCoords = 0;

	do{
		//Generation
		for(int i=0; i<game->map.nResources*2; i++){
				game->map.resources[i].pos.x=(rand()%MAP_SIZE)+1;
				game->map.resources[i].pos.y=(rand()%MAP_SIZE)+1;

				if(i < game->map.nResources/2)
					game->map.resources[i].type=GOLD;
				else
					game->map.resources[i].type=WOOD;
		}

		//Validation
		validCoords = 1;
		for(int i=0; i<game->map.nResources*2; i++){
			validCoords = !checkMap(*game, game->map.resources[i].pos);

			if(!validCoords)
				break;
		}

	}while(!validCoords);

}

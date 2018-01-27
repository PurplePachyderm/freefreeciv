#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DISTANCE_MIN 8
#define DISTANCE_MIN_RES 2
#define NBR_RESSOURCES 10
#define GEN_MAX 15
#define GOLD 0
#define WOOD 1

typedef struct
{
	int x;
	int y;
}coord;

typedef struct
{
	coord pos;
	int type;
}ressources;

void gen_points(coord* j1, coord* j2);
void gen_ressources(ressources* res);
void distances_points(coord* j1,coord* j2,ressources* res);

int main()
{
	coord joueur1,joueur2;
	ressources res_main;
	srand(time(NULL));

	distances_points(&joueur1,&joueur2,&res_main);
	system("pause");

	return 0;
}

void gen_points(coord* j1, coord* j2)
{
	int a,b,c;
	do
	{
    	j1->x=rand()%GEN_MAX;
    	j1->y=rand()%GEN_MAX;
    	j2->x=rand()%GEN_MAX;
    	j2->y=rand()%GEN_MAX;
		a=abs(j1->x-j2->x);
		b=abs(j1->y-j2->y);
		c=a+b;
    }while(j1->x==j2->x && j1->y==j2->y && c<DISTANCE_MIN);
}

void gen_ressources(ressources* res)
{
	int i=0;
	res=(ressources*)malloc(sizeof(ressources)*NBR_RESSOURCES);

	for(i=0;i<NBR_RESSOURCES;i++)
	{
			res[i].pos.x=rand()%GEN_MAX;
			res[i].pos.y=rand()%GEN_MAX;
			if(i<5)
				res[i].type=GOLD;
			else
				res[i].type=WOOD;
	}
}

void distances_points(coord* j1,coord* j2,ressources* res)
{
	int a,b,c,d,e,f;
	int i=0;
	gen_ressources(res);
	gen_points(j1,j2);
	for(i=0;i<NBR_RESSOURCES;i++)
	{
		if(i<5)
		{
			res[i].type=GOLD;
			do
			{
				a=abs(j1->x-res[i].pos.x);
				b=abs(j1->y-res[i].pos.y);
				c=abs(j2->x-res[i].pos.x);
				d=abs(j2->y-res[i].pos.y);
				e=a+b;
				f=c+d;
			}while(e<DISTANCE_MIN_RES && f<DISTANCE_MIN_RES);
		}
		else
		{
			res[i].type=WOOD;
			do
			{
				a=abs(j1->x-res[i].pos.x);
				b=abs(j1->y-res[i].pos.y);
				c=abs(j2->x-res[i].pos.x);
				d=abs(j2->y-res[i].pos.y);
				e=a+b;
				f=c+d;
			}while(e<DISTANCE_MIN_RES && f<DISTANCE_MIN_RES);
		}
	}
}

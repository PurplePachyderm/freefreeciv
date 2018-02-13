#Variables
DISPLAY_FILES = display.o map_display.o tokens_display.o menu.o hud.o hud_display.o
GAME_FILES = structures_init.o save_system.o units_actions.o game.o
LIBS = -Llib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljson-c


all: freefreeciv clean

#Executable
freefreeciv:	main.o $(DISPLAY_FILES) $(GAME_FILES)
	gcc -o freefreeciv main.o $(DISPLAY_FILES) $(GAME_FILES) -Wall $(LIBS) -lm


#Main
main.o:	main.c
	gcc -c main.c -Wall


#Display
display.o:	display/display.c
	gcc -c display/display.c -Wall

map_display.o:	display/map_display.c
	gcc -c display/map_display.c -Wall

tokens_display.o:	display/tokens_display.c
	gcc -c display/tokens_display.c -Wall

menu.o:	display/menu.c
	gcc -c display/menu.c -Wall

hud.o:	display/hud.c
	gcc -c display/hud.c -Wall

hud_display.o:	display/hud_display.c
	gcc -c display/hud_display.c -Wall


#Game
structures_init.o:
	gcc -c game/structures_init.c -Wall

save_system.o:
	gcc -c game/save_system.c -Wall

units_actions.o:
	gcc -c game/units_actions.c -Wall

game.o:
	gcc -c game/game.c -Wall


#Phony
clean:
	rm -rf *.o

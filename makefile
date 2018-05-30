#Variables
DISPLAY_FILES = display.o map_display.o tokens_display.o menu.o hud.o hud_display.o
GAME_FILES = structures_init.o save_system.o units_actions.o game.o map.o ai.o
MULTIPLAYER_FILES = easywsclient.o json.o game_init.o in_game.o multi_hud.o
LIBS = -Llib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljson-c -lcurl -lm


all: freefreeciv clean

#Executable
freefreeciv:	main.o $(DISPLAY_FILES) $(GAME_FILES) $(MULTIPLAYER_FILES)
	g++ -o freefreeciv main.o $(DISPLAY_FILES) $(GAME_FILES) $(MULTIPLAYER_FILES) -Wall $(LIBS)


#Main
main.o:	main.c
	g++ -c main.c -Wall


#Display
display.o:	display/display.c
	g++ -c display/display.c -Wall

map_display.o:	display/map_display.c
	g++ -c display/map_display.c -Wall

tokens_display.o:	display/tokens_display.c
	g++ -c display/tokens_display.c -Wall

menu.o:	display/menu.c
	g++ -c display/menu.c -Wall

hud.o:	display/hud.c
	g++ -c display/hud.c -Wall

hud_display.o:	display/hud_display.c
	g++ -c display/hud_display.c -Wall


#Game
structures_init.o:
	g++ -c game/structures_init.c -Wall

save_system.o:
	g++ -c game/save_system.c -Wall

units_actions.o:
	g++ -c game/units_actions.c -Wall

game.o:
	g++ -c game/game.c -Wall

map.o:
	g++ -c game/map.c -Wall

ai.o:
	g++ -c game/ai.c -Wall


#Multiplayer
easywsclient.o:
	g++ -c multiplayer/easywsclient.cpp -Wall

json.o:
	g++ -c multiplayer/json.c -Wall

game_init.o:
	g++ -c multiplayer/game_init.cpp -Wall

in_game.o:
	g++ -c multiplayer/in_game.cpp -Wall

multi_hud.o:
	g++ -c multiplayer/multi_hud.cpp -Wall



#Phony
clean:
	rm -rf *.o

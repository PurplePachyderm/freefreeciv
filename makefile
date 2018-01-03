#Variables
DISPLAY_FILES = display.o map_display.o tokens_display.o
LIBS = -Llib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljson-c


all: freefreeciv clean

#Executable
freefreeciv:	main.o $(DISPLAY_FILES)
	gcc -o freefreeciv main.o $(DISPLAY_FILES) -Wall $(LIBS)

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


clean:
	rm -rf *.o

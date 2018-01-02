#TODO Variables

all: freefreeciv clean

#Executable
freefreeciv:	main.o display.o
	gcc -o freefreeciv main.o display.o -Wall -Llib -lSDL2main -lSDL2 -lSDL2_image

#Main
main.o:	main.c
	pwd
	gcc -c main.c -Wall

#Display
display.o:	display/map_display.c
	gcc -o display.o -c display/map_display.c -Wall



clean:
	rm -rf *.o

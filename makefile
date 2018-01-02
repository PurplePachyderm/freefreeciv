#Variables
DISPLAY_FILES = display/map_display.c
LIBS = -Llib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljson-c


all: freefreeciv clean

#Executable
freefreeciv:	main.o display.o
	gcc -o freefreeciv main.o display.o -Wall $(LIBS)

#Main
main.o:	main.c
	pwd
	gcc -c main.c -Wall

#Display
display.o:	$(DISPLAY_FILES)
	gcc -o display.o -c $(DISPLAY_FILES) -Wall



clean:
	rm -rf *.o

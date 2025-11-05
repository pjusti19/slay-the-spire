ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=$(PATH_ALLEGRO)\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=$(PATH_ALLEGRO)\include

all: game.exe

game.exe: main.o renderer.o utils.o
	gcc -o game.exe main.o renderer.o utils.o $(LIB_ALLEGRO) -lm

main.o: main.c
	gcc -I $(INCLUDE_ALLEGRO) -c main.c

renderer.o: renderer.c
	gcc -I $(INCLUDE_ALLEGRO) -c renderer.c

utils.o: utils.c
	gcc -I $(INCLUDE_ALLEGRO) -c utils.c

clean:
	del *.o game.exe

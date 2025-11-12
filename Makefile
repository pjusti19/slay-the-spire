ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=$(PATH_ALLEGRO)\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=$(PATH_ALLEGRO)\include

all: game.exe

game.exe: main.o renderer.o utils.o player.o enemy.o card.o deck.o stats.o enemyGroup.o action.o
	gcc -o game.exe main.o renderer.o utils.o player.o enemy.o card.o deck.o stats.o enemyGroup.o $(LIB_ALLEGRO) -lm

main.o: main.c
	gcc -I $(INCLUDE_ALLEGRO) -c main.c

renderer.o: renderer.c
	gcc -I $(INCLUDE_ALLEGRO) -c renderer.c

utils.o: utils.c
	gcc -I $(INCLUDE_ALLEGRO) -c utils.c

player.o: player.c
	gcc -I $(INCLUDE_ALLEGRO) -c player.c

enemy.o: enemy.c
	gcc -I $(INCLUDE_ALLEGRO) -c enemy.c

card.o: card.c
	gcc -I $(INCLUDE_ALLEGRO) -c card.c

deck.o: deck.c
	gcc -I $(INCLUDE_ALLEGRO) -c deck.c

stats.o: stats.c
	gcc -I $(INCLUDE_ALLEGRO) -c stats.c

enemyGroup.o: enemyGroup.c
	gcc -I $(INCLUDE_ALLEGRO) -c enemyGroup.c

actions.o: actions.c
	gcc -I $(INCLUDE_ALLEGRO) -c action.c

clean:
	del *.o game.exe

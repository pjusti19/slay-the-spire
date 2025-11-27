ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=/allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=$(PATH_ALLEGRO)/lib/liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=$(PATH_ALLEGRO)/include

all: clean game.exe run


clean:
	del *.o game.exe

game.exe: main.o renderer.o utils.o player.o enemy.o card.o deck.o stats.o enemyGroup.o combat.o
	gcc -o game.exe $^ $(LIB_ALLEGRO) -lm

%.o: %.c
	gcc -I $(INCLUDE_ALLEGRO) -c $<

run: 
	.\game.exe
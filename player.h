#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "stats.h"
#include "deck.h"

typedef struct
{
  Stats *player_stats;
  Deck *deck;
} Player;

Player *createPlayer(int init_healt, int init_shield);

void freePlayer(Player *player);

#endif
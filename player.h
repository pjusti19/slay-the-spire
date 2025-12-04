#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "stats.h"
#include "deck.h"

typedef struct
{
  Stats *player_stats;
  Deck *deck;
  Deck *stack;
  Deck *hand;
  Deck *discard_stack;
  int energy;
  int max_energy;
  int charges;
} Player;

Player *createPlayer();

void freePlayer(Player *player);

#endif
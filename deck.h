#ifndef _DECK_H_
#define _DECK_H_

#include "card.h"
#include <math.h>

typedef struct{
    Card **cards;
    int deck_size;
} Deck;

Deck *createDeck();

void discardCard(Deck *deck);

#endif
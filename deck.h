#ifndef _DECK_H_
#define _DECK_H_

#include "card.h"
#include <math.h>
#include <stdbool.h>

typedef struct{
    Card **cards;
    int deck_size;
} Deck;

Deck *createDeck(int deck_size, bool __create_cards);

void buyHandCards(Deck *deck, Deck *hand, int bought_cards);

void discardCard(Deck *deck, int card_number);

#endif
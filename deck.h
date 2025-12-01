#ifndef _DECK_H_
#define _DECK_H_

#include "card.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    Card **cards;
    int deck_size;
} Deck;

Deck *createDeck(int deck_size, bool __create_cards);

void transferCards(Deck *origin, Deck *destiny);

void buyHandCards(Deck *stack, Deck *hand, Deck* discard_stack, int bought_cards);

void discardCard(Deck *hand, int *pointed_card, Deck *discard_stack, bool __remove_from_combat);

Deck *copyDeck(const Deck *src);

void freeDeckCards(Deck* deck);

#endif
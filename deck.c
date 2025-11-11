#include "deck.h"

#include "utils.h"
#include "card.h"
#include <stdlib.h>

Card **createDeck()
{
    Card **deck = (Card **)malloc(sizeof(Card *));
    if (deck == NULL)
    {
        allocFail("Deck");
        exit(1);
    }
    for (int i = 0; i < 20; i++)
    {
        if (i < 6)
            deck[i] = createCard(ATTACK, i);
        else if(i < 10)
            deck[i] = createCard(ATTACK, (rand() % 4));
        else if(i < 16)
            deck[i] = createCard(DEFENSE, (i - 10));
        else if(i < 18)
            deck[i] = createCard(DEFENSE, (rand() % 3));
        else
            deck[i] = createCard(SPECIAL, 0);
    }

    return deck;
}
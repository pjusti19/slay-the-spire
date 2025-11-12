#include "deck.h"

#include "utils.h"
#include "card.h"
#include <stdlib.h>

Deck *createDeck()
{
    Deck *deck = (Deck *)malloc(sizeof(Deck));
    if (deck == NULL)
        allocFail("Deck");

    deck->deck_size = 20;

    deck->cards = (Card **)malloc(deck->deck_size * sizeof(Card *));
    if (deck->cards == NULL)
        allocFail("Cards");

    for (int i = 0; i < deck->deck_size; i++)
    {
        if (i < 6)
            deck->cards[i] = createCard(ATTACK, ceil(pow(1.25, i) - 1));
        else if (i < 10)
            deck->cards[i] = createCard(ATTACK, (rand() % 4));
        else if (i < 16)
            deck->cards[i] = createCard(DEFENSE, ceil(pow(1.25, i - 10) - 1));
        else if (i < 18)
            deck->cards[i] = createCard(DEFENSE, (rand() % 3));
        else
            deck->cards[i] = createCard(SPECIAL, 0);
    }

    shuffleDeck(deck);

    return deck;
}

void discardCard(Deck *deck)
{
    deck->deck_size--;
}
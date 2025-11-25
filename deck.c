#include "deck.h"

#include "utils.h"
#include "card.h"
#include <stdlib.h>

Deck *createDeck(int deck_size, bool __create_cards)
{
    Deck *deck = (Deck *)malloc(sizeof(Deck));
    if (deck == NULL)
        allocFail("Deck");

    deck->deck_size = deck_size;

    if (__create_cards == true)
    {

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
    }
    else
        deck->cards = NULL; // hand initialization

    return deck;
}

void buyHandCards(Deck *deck, Deck *hand, int bought_cards)
{
    int old_hand_size = hand->deck_size;
    int new_hand_size;

    if (deck->deck_size - bought_cards < 0)
    {
        new_hand_size = old_hand_size + deck->deck_size;
        // embaralhaPilhaDescarte
    }
    else
        new_hand_size = old_hand_size + bought_cards;

    if (hand->cards == NULL)
    {
        hand->cards = (Card **)malloc(new_hand_size * sizeof(Card *));
        if (hand->cards == NULL)
            allocFail("Hand malloc");
    }
    else
    {
        Card **temp = realloc(hand->cards, new_hand_size * sizeof(Card *));
        if (temp == NULL)
            allocFail("Hand realloc");
        hand->cards = temp;
    }

    hand->deck_size = new_hand_size;

    for (int i = old_hand_size; i < new_hand_size; i++)
    {
        int bought_card = rand() % deck->deck_size;
        hand->cards[i] = deck->cards[bought_card];
        discardCard(deck, bought_card);
    }
}

void discardCard(Deck *deck, int card_number)
{
    // Moving the bought card to an inactive area of the deck
    Card *removed = deck->cards[card_number];
    deck->cards[card_number] = deck->cards[deck->deck_size - 1];
    deck->cards[deck->deck_size - 1] = removed;

    deck->deck_size--;
}

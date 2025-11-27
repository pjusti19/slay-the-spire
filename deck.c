#include "deck.h"

#include "utils.h"
#include "card.h"
#include <stdlib.h>
#include <stdio.h>

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

void transferCards(Deck *origin, Deck *destiny)
{
    int last = origin->deck_size - 1;
    for (int i = last; i >= 0; i--)
        discardCard(origin, &last, destiny);
}

void buyHandCards(Deck *stack, Deck *hand, Deck *discard_stack, int bought_cards)
{
    int old_hand_size = hand->deck_size;
    int new_hand_size;

    if (stack->deck_size == 0)
    {
        shuffleDeck(discard_stack);
        transferCards(discard_stack, stack);
    }

    if (stack->deck_size - bought_cards < 0)
    {
        new_hand_size = stack->deck_size;
        shuffleDeck(discard_stack);
        transferCards(discard_stack, stack);
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
        int bought_card = rand() % stack->deck_size;
        hand->cards[i] = stack->cards[bought_card];
        stack->cards[bought_card] = stack->cards[stack->deck_size - 1];
        stack->cards[stack->deck_size - 1] = NULL;
        stack->deck_size--;
    }
}

Deck *copyDeck(const Deck *src)
{
    Deck *copy = malloc(sizeof(Deck));
    if (copy == NULL)
        allocFail("Deck copy");

    copy->deck_size = src->deck_size;

    copy->cards = malloc(sizeof(Card *) * src->deck_size);
    if (copy->cards == NULL)
        allocFail("Deck cards copy");

    for (int i = 0; i < src->deck_size; i++)
    {
        if (src->cards[i] != NULL)
        {
            copy->cards[i] = malloc(sizeof(Card));
            if (copy->cards[i] == NULL)
                allocFail("Card copy");

            *copy->cards[i] = *src->cards[i];
        }
        else
        {
            copy->cards[i] = NULL;
        }
    }

    return copy;
}

void discardCard(Deck *hand, int *pointed_card, Deck *discard_stack)
{
    discard_stack->cards[discard_stack->deck_size] = hand->cards[*pointed_card];
    discard_stack->deck_size++;

    for (int i = *pointed_card; i < hand->deck_size - 1; i++)
    {
        Card *aux = hand->cards[i];
        hand->cards[i] = hand->cards[i+1];
        hand->cards[i+1] = aux;
    }
    hand->cards[hand->deck_size-1] = NULL;
    hand->deck_size--;

    if (*pointed_card >= hand->deck_size)
        *pointed_card = hand->deck_size - 1;
}

void freeDeckCards(Deck *deck) // used for the deck itself + hand, stack and discarded_stack
{
    for (int i = 0; i < deck->deck_size; i++)
    {
        if (deck->cards[i] != NULL)
            free(deck->cards[i]);
    }
}
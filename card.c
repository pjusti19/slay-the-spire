#include "card.h"
#include "utils.h"
#include <stdlib.h>

Card *createCard(ActionType card_type, int cost)
{
    Card *card = (Card *)malloc(sizeof(Card));
    if (card == NULL)
        allocFail("Card");

    card->card_type = card_type;
    if (card_type == SPECIAL)
    {
        card->cost = 0;
        card->effect_rate = 0;
    }
    else // DEFENSE & ATTACK
    {
        card->cost = cost;
        switch (card->cost)
        {
        case 0:
            card->effect_rate = 1 + rand() % 5;
            break;
        case 1:
            card->effect_rate = 5 + rand() % 6;
            break;
        case 2:
            card->effect_rate = 10 + rand() % 6;
            break;
        case 3:
            card->effect_rate = 15 + rand() % 16;
            break;
        }
    }

    return card;
}
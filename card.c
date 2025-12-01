#include "card.h"
#include "utils.h"
#include <stdlib.h>

Card *createCard(ActionType card_type, int cost)
{
    Card *card = (Card *)malloc(sizeof(Card));
    if (card == NULL)
        allocFail("Card");

    card->card_type = card_type;
    card->cost = cost;
    if (card_type == SPECIAL)
        card->effect_rate = 0;
    else if(card_type == LIFESTEAL)
        card->effect_rate = BASE_LIFESTEAL_EFFECT + rand() % 11;
    else if(card_type == STRENGTH || card_type == DEXTERITY)    
        card->effect_rate = 3 * cost;
    else if(card_type == VULNERABILITY)    
        card->effect_rate = cost * 20;
    else if(card_type == WEAKNESS)
        card->effect_rate = cost * 8;
    else if(card_type == POISON)
        card->effect_rate = cost * 3;
    else // DEFENSE & ATTACK
    {
        
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
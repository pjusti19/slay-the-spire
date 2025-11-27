#ifndef _CARD_H_
#define _CARD_H_

#include <stdbool.h>

typedef enum
{
    DEFENSE = 1,
    ATTACK = 2,
    SPECIAL = 3,
} ActionType;

typedef struct
{
    int cost;
    int effect_rate;
    ActionType card_type;
} Card;

Card *createCard(ActionType card_type, int cost);

#endif
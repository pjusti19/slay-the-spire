#ifndef _CARD_H_
#define _CARD_H_

#include <stdbool.h>

typedef enum
{
    DEFENSE = 1,
    ATTACK = 2,
    LIFESTEAL = 3,
    STRENGTH = 4,
    DEXTERITY = 5,
    VULNERABILITY = 6,
    WEAKNESS = 7,
    POISON = 8,
    SPECIAL = 9,
} ActionType;

typedef struct
{
    int cost;
    int effect_rate;
    ActionType card_type;
} Card;

Card *createCard(ActionType card_type, int cost);

#endif
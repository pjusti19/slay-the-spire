#ifndef _CARD_H_
#define _CARD_H_

#include <stdbool.h>

typedef enum
{
    DEFENSE = 1,
    ATTACK = 2,
    POISON = 3,
    VULNERABILITY = 4,
    WEAKNESS = 5,
    LIFESTEAL = 6,
    STRENGTH = 7,
    DEXTERITY = 8,
    SPECIAL = 9,
    KAIOKEN = 10,
    CHARGE = 11,
} ActionType;

typedef struct
{
    int cost;
    int effect_rate;
    bool __is_skipable;
    ActionType card_type;
} Card;

Card *createCard(ActionType card_type, int cost);

#endif
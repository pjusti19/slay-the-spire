#ifndef _CARD_H_
#define _CARD_H_

typedef enum
{
    DEFENSE = 1,
    ATTACK = 2,
    SPECIAL = 3,
} CardType;

typedef struct
{
    int cost;
    int effect_rate;
    CardType card_type;
} Card;

Card *createCard(CardType card_type, int cost);

#endif
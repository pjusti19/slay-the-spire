#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "enemy.h"

typedef enum
{
    DEFENSE = 1,
    ATTACK = 2,
} ActionType;

typedef struct{
    ActionType action_type;
    int effect_rate;
} Action;

Action *createAction(ActionType action_type, int cost);

#endif
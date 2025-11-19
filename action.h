#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "card.h"

typedef struct{
    ActionType action_type;
    int effect_rate;
} Action;

Action *createAction(ActionType action_type, int cost);

#endif
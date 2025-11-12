#include "action.h"

#include "utils.h"
#include <stdlib.h>

Action *createAction(ActionType action_type, int cost)
{
    Action *action = (Action *)malloc(sizeof(Action));
    if (action == NULL)
        allocFail("Action");

    action->action_type = action_type;
    switch(cost){
        {
        case 0:
            action->effect_rate = 1 + rand() % 5;
            break;
        case 1:
            action->effect_rate = 5 + rand() % 6;
            break;
        case 2:
            action->effect_rate = 10 + rand() % 6;
            break;
        case 3:
            action->effect_rate = 15 + rand() % 16;
            break;
        }
    }

    return action;
}
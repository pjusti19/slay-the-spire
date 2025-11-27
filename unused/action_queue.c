#include "action_queue.h"
#include "stdlib.h"
#include "utils.h"

Action *createAction(Card *card, int card_position, int pointed_enemy, Action *next_action)
{
    Action *action = (Action *)malloc(sizeof(Action));
    if (action == NULL)
        allocFail("Action");

    action->card = card;
    action->card_position = card_position;
    action->pointed_enemy = pointed_enemy;
    action->next_action = next_action;

    return action;
}

ActionQueue *createActionQueue()
{
    ActionQueue *queue = (ActionQueue *)malloc(sizeof(ActionQueue));
    if (queue != NULL)
    {
        queue->lenght = 0;
        return queue;
    }
    else
        return NULL;
}

void enqueueAction(ActionQueue *queue, Card *card, int card_position, int pointed_enemy)
{
    Action *action = createAction(card, card_position, pointed_enemy, NULL);
    if (queue->lenght == 0)
        queue->end = queue->beginning = action;
    else
    {
        queue->end->next_action = action;
        queue->end = action;
    }
    queue->lenght++;
}

Action *dequeueAction(ActionQueue *queue)
{
    if (queue->lenght == 0)
        return NULL;

    Action *dequeued_action = queue->beginning;
    queue->beginning = queue->beginning->next_action;

    queue->lenght--;

    if (queue->lenght == 0)
        queue->end = NULL;

    return dequeued_action;
}
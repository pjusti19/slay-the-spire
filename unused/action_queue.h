#ifndef _ACTION_QUEUE_H_
#define _ACTION_QUEUE_H_

#include "card.h"

typedef struct Action{
    Card *card;
    int pointed_enemy;
    int card_position;
    struct Action *next_action;
} Action;

typedef struct ActionQueue{
    Action *beginning, *end;
    int lenght;
} ActionQueue;

Action *createAction(Card *card, int card_position, int pointed_enemy, Action *next_action);

ActionQueue *createActionQueue();

void enqueueAction(ActionQueue *queue, Card *card, int card_position, int pointed_enemy);

Action *dequeueAction(ActionQueue *queue);

#endif
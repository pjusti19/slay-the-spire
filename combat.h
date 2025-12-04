#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "player.h"
#include "enemyGroup.h"
#include <stdbool.h>

typedef struct{
    Player *player;
    EnemyGroup *enemy_group;
    bool __is_card_selected;
    bool __has_enter_been_pressed;
    bool __has_passive_card_been_used;
    int pointed_card;
    int pointed_enemy;
    int enemies_left;
} Combat;

Combat *createCombat(Player *player, int enemy_amount);

void beginPlayerTurn(Combat *combat);

void applyAction(Combat *combat, Card *used_card, Stats *caster, Stats *target);
    
void enemyTurn(Combat *combat);

void freeCombat(Combat *combat);

void updateChargeCard(Deck *hand, int actual_energy);

#endif
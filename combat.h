#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "player.h"
#include "enemyGroup.h"
#include <stdbool.h>

typedef struct{
    Player *player;
    EnemyGroup *enemy_group;
    bool __is_player_turn;
    bool __is_card_selected;
    int pointed_card;
    int pointed_enemy;
    int enemies_left;
} Combat;

Combat *createCombat(Player *player, int enemy_amoun);

void beginPlayerTurn(Combat *combat);

void beginEnemyTurn(Combat *combat);

#endif
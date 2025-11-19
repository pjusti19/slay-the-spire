#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "player.h"
#include "enemyGroup.h"
#include <stdbool.h>

typedef struct{
    Player *player;
    EnemyGroup *enemy_group;
    bool __is_player_turn;
    int enemies_left;
} Combat;

Combat *createCombat(int enemy_amount);

#endif
#ifndef _ENEMY_GROUP_H_
#define _ENEMY_GROUP_H_

#include "enemy.h"

typedef struct{
    Enemy **enemies;
    int enemies_amount;
} EnemyGroup;

EnemyGroup *createEnemyGroup(int enemies_amount);

#endif
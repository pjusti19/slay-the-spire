#ifndef _ENEMY_GROUP_H_
#define _ENEMY_GROUP_H_

#include "enemy.h"

typedef struct{
    Enemy **enemies;
    int enemy_amount;
} EnemyGroup;

EnemyGroup *createEnemyGroup(int enemy_amount);

void freeEnemyGroup(EnemyGroup *enemy_group);

#endif
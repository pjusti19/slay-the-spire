#include "enemyGroup.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"

EnemyGroup *createEnemyGroup(int enemy_amount)
{

    EnemyGroup *enemy_group = (EnemyGroup *)malloc(sizeof(EnemyGroup));
    if (enemy_group == NULL)
        allocFail("Enemy Group");
    enemy_group->enemy_amount = enemy_amount;
    enemy_group->enemies = (Enemy **)malloc(enemy_group->enemy_amount * sizeof(Enemy *));
    if (enemy_group->enemies == NULL)
        allocFail("Enemy group enemies");
    int drawed_number = -1;
    bool __has_created_strong_enemy = false;
    for (int i = 0; i < enemy_group->enemy_amount; i++)
    {
        if (__has_created_strong_enemy == false)
        {
            drawed_number = 1 + rand() % 100;
            if (drawed_number <= 5)
            {
                enemy_group->enemies[i] = createEnemy(STRONG);
                __has_created_strong_enemy = true;
                continue;
            }
        }

        enemy_group->enemies[i] = createEnemy(WEAK);
    }

    return enemy_group;
}

void freeEnemyGroup(EnemyGroup *enemy_group)
{
    for (int i = 0; i < enemy_group->enemy_amount; i++)
    {
        free(enemy_group->enemies[i]->enemy_stats);
        for (int j = 0; j < enemy_group->enemies[i]->number_of_actions; j++)
            free(enemy_group->enemies[i]->actions[j]);
    }
}
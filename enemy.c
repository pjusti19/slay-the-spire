#include "enemy.h"

#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>

Enemy *createEnemy(EnemyType enemy_type)
{
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  if (enemy == NULL)
    allocFail("Enemy");

  enemy->enemy_type = enemy_type;

  int i = 0;
  switch (enemy->enemy_type)
  {
  case WEAK:
    enemy->enemy_stats = createStats(10 + rand() % 21, 0);
    enemy->number_of_actions = 1 + rand() % 2;
    break;
  case STRONG:
    enemy->enemy_stats = createStats(40 + rand() % 61, 0);
    enemy->number_of_actions = 2 + rand() % 2;
    break;
  }
  enemy->actions = (Action **)malloc(enemy->number_of_actions * sizeof(Action *));
  if (enemy->actions == NULL)
    allocFail("Actions");

  bool __has_created_attack_action = false;
  bool __has_created_cost1_action = false; // for strong enemy
  int action_cost;
  for (int i = 0; i < enemy->number_of_actions; i++)
  {
    switch (enemy->enemy_type)
    {
    case WEAK:
      action_cost = rand() % 2;
      break;
    case STRONG:
      if (__has_created_cost1_action == true)
        action_cost = 2 + rand() % 2;
      else
      {
        action_cost = 1 + rand() % 3;
        if (action_cost == 1)
          __has_created_cost1_action = true;
      }
    }
    if (i == enemy->number_of_actions - 1 && __has_created_attack_action == false)
      enemy->actions[i] = createAction(ATTACK, action_cost);
    else
    {
      enemy->actions[i] = createAction(1 + rand() % 2, action_cost); // ATTACK | DEFENSE
      __has_created_attack_action = true;
    }
  }

  return enemy;
}

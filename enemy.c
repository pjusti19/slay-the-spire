#include "enemy.h"

#include "utils.h"
#include <stdlib.h>
#include <stdbool.h>

Enemy *createEnemy(EnemyType enemy_type)
{
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  if (enemy == NULL)
    allocFail("Enemy");

  enemy->actions = (Deck *)malloc(sizeof(Deck));
  if (enemy->actions == NULL)
    allocFail("Actions");

  enemy->enemy_type = enemy_type;
  enemy->actual_action = 0;

  int i = 0;
  switch (enemy->enemy_type)
  {
  case WEAK:
    enemy->enemy_stats = createStats(10 + rand() % 21, 0);
    enemy->enemy_stats->entity_type = ENEMY;
    enemy->actions->deck_size = 1 + rand() % 2;
    break;
  case STRONG:
    enemy->enemy_stats = createStats(40 + rand() % 61, 0);
    enemy->actions->deck_size = 2 + rand() % 2;
    break;
  }

  enemy->actions->cards = (Card **)malloc(enemy->actions->deck_size * sizeof(Card *));
  if (enemy->actions->cards == NULL)
    allocFail("Enemy Actions");

  bool __has_created_attack_action = false;
  bool __has_created_cost1_action = false; // for strong enemy
  int action_cost;
  for (int i = 0; i < enemy->actions->deck_size; i++)
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
    if (i == enemy->actions->deck_size - 1 && __has_created_attack_action == false)
      enemy->actions->cards[i] = createCard(ATTACK, action_cost);
    else
    {
      enemy->actions->cards[i] = createCard(1 + rand() % 2, action_cost); // ATTACK | DEFENSE
      __has_created_attack_action = true;
    }
  }

  return enemy;
}

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
    enemy->enemy_stats = createStats(10 + rand() % 21, 0, DEAFULT_INIT_LIFESTEAL);
    enemy->actions->deck_size = 1 + rand() % 2;
    break;
  case STRONG:
    enemy->enemy_stats = createStats(40 + rand() % 61, 0, DEAFULT_INIT_LIFESTEAL);
    enemy->actions->deck_size = 2 + rand() % 2;
    break;
  case BOSS:
    enemy->enemy_stats = createStats(100 + rand() % 51, 0, DEFAULT_BOSS_LIFESTEAL);
    enemy->enemy_stats->dexterity = 7;
    enemy->enemy_stats->strength = 7;
    enemy->actions->deck_size = DEFAULT_BOSS_HAND_SIZE;
    break;
  }

  enemy->enemy_stats->entity_type = ENEMY;
  enemy->actions->cards = (Card **)malloc(enemy->actions->deck_size * sizeof(Card *));
  if (enemy->actions->cards == NULL)
    allocFail("Enemy Actions");

  bool __has_created_attack_action = false;
  bool __has_created_cost1_action = false; // for strong enemy
  int action_cost;
  int random_type;
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
    case BOSS:
      action_cost = 2 + rand() % 2;
      break;
    }
    if (enemy->enemy_type <= 2) // weak n strong
    {
      if ((i == enemy->actions->deck_size - 1) && (__has_created_attack_action == false))
      {
        enemy->actions->cards[i] = createCard(ATTACK, action_cost);
        __has_created_attack_action = true;
      }
      else
      {
        random_type = 1 + rand() % 5;
        if (random_type != DEFENSE && random_type != ATTACK) // Why? -> Some extra cards get no effect if cost == 0
          action_cost = DEFAULT_ENEMY_BUFF_COST;
        enemy->actions->cards[i] = createCard(random_type, action_cost); // ATTACK | DEFENSE | EXTRAS
      }
    }
    else
    {
      if (i == 0)
      {
        enemy->actions->cards[i] = createCard(DEFENSE, action_cost);
      }
      else if (i > 0 && i < enemy->actions->deck_size - 1)
      {
        enemy->actions->cards[i] = createCard(ATTACK, action_cost);
      }
      else
      {
        random_type = 3 + rand() % 3; 
        enemy->actions->cards[i] = createCard(random_type, action_cost);
      }
    }
  }
  shuffleDeck(enemy->actions);

  return enemy;
}

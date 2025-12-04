#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "stats.h"
#include "deck.h"

typedef enum
{
  WEAK = 1,
  STRONG = 2,
  BOSS = 3,
} EnemyType;

typedef struct
{
  Stats *enemy_stats;
  EnemyType enemy_type;
  Deck *actions;
  int actual_action;
  
} Enemy;

Enemy *createEnemy(EnemyType enemy_type);

#endif
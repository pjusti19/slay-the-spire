#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "stats.h"
#include "action.h"

typedef enum
{
  WEAK = 1,
  STRONG = 2,
  //  TITAN = 3 -> implementar ao final do tp
} EnemyType;

typedef struct
{
  Stats *enemy_stats;
  EnemyType enemy_type;
  Action **actions;
  int number_of_actions;
  
} Enemy;

Enemy *createEnemy(EnemyType enemy_type);

#endif
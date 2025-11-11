#ifndef _ENEMY_H_
#define _ENEMY_H_

typedef enum
{
  WEAK = 1,
  STRONG = 2,
  //  TITAN = 3 -> implementar ao final do tp
} EnemyType;

typedef struct
{
  int healthbar;
  int shieldbar;
  EnemyType type;
} Enemy;

#endif
#include "enemy.h"
#include "utils.h"
#include <stdlib.h>

Enemy *createEnemy(EnemyType type)
{
  Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
  if (enemy == NULL)
  {
    allocFail("Enemy");
    switch (type)
    {
    case WEAK:
      enemy->healthbar = 10 + rand() % 30;
      enemy->type = type;
      break;
    case STRONG:
      enemy->healthbar = 40 + rand() % 100;
      enemy->type = type;
      break;
    }
    enemy->shieldbar = 0;
    return enemy;
  }

  
}
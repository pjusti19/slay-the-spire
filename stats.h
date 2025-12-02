#ifndef _STATS_H_
#define _STATS_H_

#include <stdbool.h>

typedef enum{
    PLAYER = 1,
    ENEMY = 2,
} EntityType;

typedef struct{
    int healthbar;
    int shieldbar;
    int max_health;
    int lifesteal;
    int strength;
    int dexterity;
    int weakness;
    int vulnerability;
    int poison;
    bool __is_kaioken_active;
    EntityType entity_type;
} Stats;

Stats *createStats(int initial_health, int initial_shield, int initial_lifesteal);

#endif
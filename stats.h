#ifndef _STATS_H_
#define _STATS_H_

typedef enum{
    PLAYER = 1,
    ENEMY = 2,
} EntityType;

typedef struct{
    int healthbar;
    int shieldbar;
    int max_health;
    EntityType entity_type;
} Stats;

Stats *createStats(int initial_health, int initial_shield);

#endif
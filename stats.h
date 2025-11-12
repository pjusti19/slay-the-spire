#ifndef _STATS_H_
#define _STATS_H_

typedef struct{
    int healthbar;
    int shieldbar;
} Stats;

Stats *createStats(int initial_health, int initial_shield);

#endif
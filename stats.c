#include "stats.h"

#include <stdlib.h>
#include "utils.h"

Stats *createStats(int initial_health, int initial_shield)
{
    Stats *stats = (Stats *)malloc(sizeof(Stats));
    if (stats == NULL)
        allocFail("Stats");
    
    stats->max_health = initial_health;
    stats->healthbar = initial_health;
    stats->shieldbar = initial_shield;

    return stats;
}
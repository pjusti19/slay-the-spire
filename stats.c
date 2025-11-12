#include "stats.h"

#include <stdlib.h>
#include "utils.h"

Stats *createStats(int initial_health, int initial_shield)
{
    Stats *stats = (Stats *)malloc(sizeof(Stats));
    if (stats == NULL)
        allocFail("Stats");
        
    stats->healthbar = initial_health;
    stats->shieldbar = initial_shield;

    return stats;
}
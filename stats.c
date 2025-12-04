#include "stats.h"

#include <stdlib.h>
#include "utils.h"

Stats *createStats(int initial_health, int initial_shield, int initial_lifesteal)
{
    Stats *stats = (Stats *)malloc(sizeof(Stats));
    if (stats == NULL)
        allocFail("Stats");
    
    stats->max_health = initial_health;
    stats->healthbar = initial_health;
    stats->shieldbar = initial_shield;
    stats->lifesteal = initial_lifesteal;
    stats->strength = 0;
    stats->dexterity = 0;
    stats->vulnerability = 0;
    stats->weakness = 0;
    stats->poison = 0;
    stats->__is_kaioken_active = false;

    return stats;
}
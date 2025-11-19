#include "combat.h"

#include "utils.h"
#include "constants.h"

Combat *createCombat(int enemy_amount){
    Combat *combat = (Combat *) malloc(sizeof(Combat));
    if(combat == NULL)
        allocFail("Combat");

    combat->player = createPlayer(PLAYER_MAX_HEALTH, PLAYER_INIT_SHIELD);
    combat->enemy_group = createEnemyGroup(enemy_amount);
    combat->enemies_left = enemy_amount;
    combat->__is_player_turn = true;

    return combat;
}
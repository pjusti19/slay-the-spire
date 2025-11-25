#include "combat.h"

#include "utils.h"
#include "constants.h"
#include <stdio.h>

Combat *createCombat(Player *player, int enemy_amount)
{
    Combat *combat = (Combat *)malloc(sizeof(Combat));
    if (combat == NULL)
        allocFail("Combat");

    combat->player = player;
    combat->enemy_group = createEnemyGroup(enemy_amount);
    combat->enemies_left = enemy_amount;
    combat->__is_player_turn = true;
    combat->__is_card_selected = false;
    combat->pointed_card = 0;
    combat->pointed_enemy = -1;

    return combat;
}

void beginPlayerTurn(Combat *combat)
{
    combat->player->energy = 3;
    combat->player->player_stats->shieldbar = 0;
    combat->pointed_card = 0;
    combat->pointed_enemy = -1;
    buyHandCards(combat->player->deck, combat->player->hand, DEFAULT_HAND_STACK);
}
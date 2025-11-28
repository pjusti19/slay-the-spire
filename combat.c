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
    player->stack = copyDeck(player->deck);
    combat->player->hand->deck_size = 0;
    combat->player->discard_stack->deck_size = 0;
    beginPlayerTurn(combat);

    return combat;
}

void beginPlayerTurn(Combat *combat)
{
    combat->player->energy = 3;
    combat->player->player_stats->shieldbar = 0;
    combat->pointed_card = 0;
    combat->pointed_enemy = getFirstAliveEnemy(combat->enemy_group);
    combat->__is_card_selected = false;
    combat->__has_enter_been_pressed = false;
    combat->__has_passive_card_been_used = false;
    buyHandCards(combat->player->stack, combat->player->hand, combat->player->discard_stack, DEFAULT_HAND_STACK);
}

void enemyTurn(Combat *combat)
{
    for (int i = 0; i < combat->enemy_group->enemy_amount; i++)
    {
        combat->enemy_group->enemies[i]->enemy_stats->shieldbar = 0;
        int *actual_action = &combat->enemy_group->enemies[i]->actual_action;
        if (combat->enemy_group->enemies[i]->enemy_stats->healthbar > 0)
        {
            applyAction(combat, combat->enemy_group->enemies[i]->actions->cards[*actual_action], combat->enemy_group->enemies[i]->enemy_stats, combat->player->player_stats);
            if (*actual_action >= combat->enemy_group->enemies[i]->actions->deck_size - 1)
                *actual_action = 0;
            else
                (*actual_action)++;

            printf("acao:%d\n", *actual_action);
        }
    }
    beginPlayerTurn(combat);
}

void applyAction(Combat *combat, Card *used_card, Stats *caster, Stats *target)
{
    int damage = 0;
    switch (used_card->card_type)
    {
    case DEFENSE:
        caster->shieldbar += used_card->effect_rate;
        printf("\ndefendeu\n");
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack);
        break;
    case ATTACK:
        printf("\natacou\n");
        damage = used_card->effect_rate;
        if (target->shieldbar > 0)
        {
            target->shieldbar -= damage;
            damage = 0;
            if (target->shieldbar < 0)
            {
                damage = -(target->shieldbar);
                target->shieldbar = 0;
            }
        }
        if (damage > 0)
            target->healthbar -= damage;
        if (target->healthbar <= 0)
        {
            printf("entrou\n");
            target->healthbar = 0;
            printf("tipo: %d\n", target->entity_type);
            if (target->entity_type == ENEMY)
            {
                combat->enemies_left--;
                printf("faltam: %d\n", combat->enemies_left);
            }
        }
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack);
        break;
    case SPECIAL:
        while (combat->player->hand->deck_size > 0)
            transferCards(combat->player->hand, combat->player->discard_stack);
        buyHandCards(combat->player->stack, combat->player->hand, combat->player->discard_stack, DEFAULT_HAND_STACK);
        break;
    }
}

void freeCombat(Combat *combat)
{
    freeEnemyGroup(combat->enemy_group);
    freeDeckCards(combat->player->stack);
    freeDeckCards(combat->player->hand);
    freeDeckCards(combat->player->discard_stack);
    free(combat);
}

// void applyAction(Combat *combat, Card* used_card)
// {
//     Action *executed_action;
//     int damage = 0;
//     while ((executed_action = dequeueAction(combat->action_queue)) != NULL)
//     {
//         if (executed_action != NULL)
//         {
//             switch (executed_action->card->card_type)
//             {
//             case DEFENSE:
//                 combat->player->player_stats->shieldbar += executed_action->card->effect_rate;
//                 discardCard(combat->player->hand, executed_action->card_position, combat->player->discard_stack, executed_action->card);
//                 break;
//             case ATTACK:
//                 damage = executed_action->card->effect_rate;
//                 if (combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->shieldbar > 0)
//                 {
//                     combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->shieldbar -= damage;
//                     damage = 0;
//                     if (combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->shieldbar < 0)
//                     {
//                         damage = -(combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->shieldbar);
//                         combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->shieldbar = 0;
//                     }
//                 }
//                 if (damage > 0)
//                     combat->enemy_group->enemies[executed_action->pointed_enemy]->enemy_stats->healthbar -= damage;
//                 discardCard(combat->player->hand, executed_action->card_position, combat->player->discard_stack, executed_action->card);
//                 break;
//             case SPECIAL:
//                 for (int i = combat->player->hand->deck_size - 1; i >= 0; i--)
//                     discardCard(combat->player->hand, i, combat->player->discard_stack, combat->player->hand->cards[i]);
//             }
//         }
//     }
//     printf("escudo:%d\n", combat->player->player_stats->shieldbar);
// }
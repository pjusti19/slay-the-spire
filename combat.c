#include "combat.h"

#include "utils.h"
#include "constants.h"
#include <stdio.h>

void resetEffects(Stats *stats)
{
    stats->strength = 0;
    stats->dexterity = 0;
    stats->vulnerability = 1;
    stats->weakness = 1;
    stats->poison = 0;
    stats->__is_kaioken_active = false;
}

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
    resetEffects(combat->player->player_stats);
    beginPlayerTurn(combat);
    return combat;
}

void beginPlayerTurn(Combat *combat)
{
    combat->player->energy = combat->player->max_energy;
    combat->player->player_stats->shieldbar = 0;
    if (combat->player->player_stats->poison > 0)
    {
        combat->player->player_stats->healthbar -= combat->player->player_stats->poison;
        combat->player->player_stats->poison--;
    }
    if (combat->player->player_stats->__is_kaioken_active == true)
    {
        combat->player->player_stats->healthbar -= combat->player->player_stats->max_health * (BASE_KAIOKEN_BUFF / 2000.0);
    }
    combat->pointed_card = 0;
    combat->pointed_enemy = getFirstAliveEnemy(combat->enemy_group);
    combat->__is_card_selected = false;
    combat->__has_enter_been_pressed = false;
    combat->__has_passive_card_been_used = false;
    buyHandCards(combat->player->stack, combat->player->hand, combat->player->discard_stack, DEFAULT_HAND_STACK);
    updateChargeCard(combat->player->hand, combat->player->energy);
}

void getNextEnemyAction(Enemy *enemy, int *actual_action)
{
    if (*actual_action >= enemy->actions->deck_size - 1)
        *actual_action = 0;
    else
        (*actual_action)++;

    if (enemy->actions->cards[*actual_action]->__is_skipable == true)
        getNextEnemyAction(enemy, actual_action);

    return;
}

void checkSkipableAction(Card *action)
{
    if (action->card_type != ATTACK && action->card_type != DEFENSE && action->card_type != POISON)
    {
        action->__is_skipable = true;
        printf("marcou\n");
    }
}

void enemyTurn(Combat *combat)
{
    for (int i = 0; i < combat->enemy_group->enemy_amount; i++)
    {
        combat->enemy_group->enemies[i]->enemy_stats->shieldbar = 0;
        if (combat->enemy_group->enemies[i]->enemy_stats->poison > 0)
        {
            combat->enemy_group->enemies[i]->enemy_stats->healthbar -= combat->enemy_group->enemies[i]->enemy_stats->poison;
            combat->enemy_group->enemies[i]->enemy_stats->poison--;
            if (combat->enemy_group->enemies[i]->enemy_stats->healthbar <= 0)
            {
                combat->enemy_group->enemies[i]->enemy_stats->healthbar = 0;
                combat->enemies_left--;
            }
        }
        int *actual_action = &combat->enemy_group->enemies[i]->actual_action;
        Card *action = combat->enemy_group->enemies[i]->actions->cards[*actual_action];
        if (action->__is_skipable == true)
            getNextEnemyAction(combat->enemy_group->enemies[i], actual_action);

        if (combat->enemy_group->enemies[i]->enemy_stats->healthbar > 0)
        {
            applyAction(combat, action, combat->enemy_group->enemies[i]->enemy_stats, combat->player->player_stats);
            checkSkipableAction(action);
            getNextEnemyAction(combat->enemy_group->enemies[i], actual_action);
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
        caster->shieldbar += ceil((used_card->effect_rate * (double)(1.0 + caster->dexterity / 100.0)));
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, false);
        break;
    case ATTACK:
        damage = ceil((used_card->effect_rate * (double)(1.0 + caster->strength / 100.0)) *
                      (double)(1.0 + target->vulnerability / 100.0) *
                      (double)(1.0 - caster->weakness / 100.0));
        if (damage > 0 && caster->lifesteal > 0)
        {
            int healed = floor(damage * (caster->lifesteal / 100.0));
            caster->healthbar += healed;

            if (caster->healthbar > caster->max_health) // No overheal!
                caster->healthbar = caster->max_health;
        }
        int postShieldDamage = damage;

        if (target->shieldbar > 0)
        {
            target->shieldbar -= postShieldDamage;
            postShieldDamage = 0;

            if (target->shieldbar < 0)
            {
                postShieldDamage = -(target->shieldbar);
                target->shieldbar = 0;
            }
        }
        if (postShieldDamage > 0)
            target->healthbar -= postShieldDamage;

        if (target->healthbar <= 0)
        {
            target->healthbar = 0;
            if (target->entity_type == ENEMY)
                combat->enemies_left--;
        }
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, false);

        break;
    case SPECIAL:
        while (combat->player->hand->deck_size > 0)
            transferCards(combat->player->hand, combat->player->discard_stack);
        buyHandCards(combat->player->stack, combat->player->hand, combat->player->discard_stack, DEFAULT_HAND_STACK);
        break;
    case LIFESTEAL:
        caster->lifesteal = used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case STRENGTH:
        caster->strength += used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case DEXTERITY:
        caster->dexterity += used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case VULNERABILITY:
        target->vulnerability += used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case WEAKNESS:
        target->weakness += used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case POISON:
        target->poison += used_card->effect_rate;
        if (caster->entity_type == PLAYER)
            discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    case KAIOKEN:
        caster->strength += used_card->effect_rate;
        caster->__is_kaioken_active = true;
        discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
    case CHARGE:
        combat->player->charges += used_card->cost;
        if (combat->player->charges >= 5)
        {
            combat->player->charges = 0;
            caster->max_health += CHARGE_HEALTHBAR_BUFF;
            caster->healthbar += CHARGE_HEALTHBAR_BUFF;
            combat->player->max_energy += CHARGE_ENERGY_BUFF;
            combat->player->energy += CHARGE_ENERGY_BUFF;
        }
        discardCard(combat->player->hand, &combat->pointed_card, combat->player->discard_stack, true);
        break;
    }
}

void updateChargeCard(Deck *hand, int actual_energy)
{
    for (int i = 0; i < hand->deck_size; i++)
    {
        if (hand->cards[i]->card_type == CHARGE)
        {
            hand->cards[i]->effect_rate = hand->cards[i]->cost = actual_energy;
            break;
        }
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
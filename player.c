#include "player.h"
#include "constants.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

Player *createPlayer()
{
  Player *player = (Player *)malloc(sizeof(Player));
  if (player == NULL)
    allocFail("Player");

  player->energy = player->max_energy = INITIAL_MAX_ENERGY;
  player->charges = 0;
  player->player_stats = createStats(PLAYER_MAX_HEALTH, PLAYER_INIT_SHIELD, DEAFULT_INIT_LIFESTEAL);
  player->player_stats->entity_type = PLAYER;
  // player->deck = createDeck(MAX_DECK_STACK, true);
  player->deck = createDeck(EXTRA_DECK_STACK, true);
  player->hand = createDeck(0, false);
  player->discard_stack = createDeck(0, false);
  player->discard_stack->cards = (Card **)malloc(EXTRA_DECK_STACK * sizeof(Card *));
  player->discard_stack->deck_size = 0;
  return player;
}

void freePlayer(Player *player)
{
  freeDeckCards(player->deck);
  free(player->deck);
  free(player->stack);
  free(player->hand);
  free(player->discard_stack);
  free(player->player_stats);
  free(player);
}
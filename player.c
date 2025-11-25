#include "player.h"
#include "constants.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

Player *createPlayer(int init_health, int init_shield)
{
  Player *player = (Player *)malloc(sizeof(Player));
  if (player == NULL)
    allocFail("Player");

  player->energy = 0;
  player->player_stats = createStats(init_health, init_shield);
  player->deck = createDeck(MAX_DECK_STACK, true);
  player->hand = createDeck(0, false);
  buyHandCards(player->deck, player->hand, DEFAULT_HAND_STACK); // será movido pro "beginTurn" do combat.c quando terminar de implementar a função
  return player;
}

void freePlayer(Player *player)
{
  // deck
  for (int i = 0; i < player->deck->deck_size; i++)
  {
    if (player->deck->cards[i] != NULL)
      free(player->deck->cards[i]);
  }
  free(player->deck->cards);
  free(player->deck);
  // hand
  for (int i = 0; i < player->hand->deck_size; i++)
  {
    if (player->hand->cards[i] != NULL)
      free(player->hand->cards[i]);
  }
  free(player->hand->cards);
  free(player->hand);
  //
  free(player->player_stats);
  free(player);
}
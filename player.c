#include "player.h"
#include "utils.h"
#include <stdlib.h>

Player *createPlayer(int init_health, int init_shield)
{
  Player *player = (Player *)malloc(sizeof(Player));
  if (player == NULL)
    allocFail("Player");

  player->player_stats = createStats(init_health, init_shield);
  player->deck = createDeck();
  return player;
}

void freePlayer(Player *player)
{
  for (int i = 0; i < player->deck->deck_size; i++)
  {
    if (player->deck->cards[i] != NULL)
      free(player->deck->cards[i]);
  }
  free(player->deck->cards);
  free(player->deck);
  free(player->player_stats);
  free(player);
}
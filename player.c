#include "player.h"
#include "utils.h"
#include <stdlib.h>

Player *createPlayer()
{
  Player *player = (Player *)malloc(sizeof(Player));
  if (player == NULL)
  {
    allocFail("Player");
    exit(1);
  }
  player->healthbar = 100;
  player->shieldbar = 0;
  return player;
}
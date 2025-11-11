#ifndef _PLAYER_H_
#define _PLAYER_H_

typedef struct
{
  int healthbar;
  int shieldbar;
  // Deck deck; -> implementar quando for mexer com o deck
} Player;

Player *createPlayer();

#endif
#include "utils.h"
#include "constants.h"
#include "deck.h"

#include <allegro5/allegro5.h>
#include <stdio.h>

void must_init(_Bool test, const char *description)
{
  if (test)
    return;

  fprintf(stderr, "couldn't initialize %s\n", description);
  exit(1);
}

int NumToDigits(int n)
{
  if (n < 0)
    return 1 + NumToDigits(-n);
  if (n < 10)
    return 1;
  if (n < 100)
    return 2;
  if (n < 1000)
    return 3;
  if (n < 10000)
    return 4;
  if (n < 100000)
    return 5;
  if (n < 1000000)
    return 6;
  if (n < 10000000)
    return 7;
  if (n < 100000000)
    return 8;
  if (n < 1000000000)
    return 9;
  /*      2147483647 is 2^31-1 - add more ifs as needed
     and adjust this final return as well. */
  return 10;
}

void ClearKeyboardKeys(unsigned char *keyboard_keys)
{
  memset(keyboard_keys, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));
}

void ShuffleArray(int *array, int size)
{
  if (size > 1)
  {
    int i;
    for (i = 0; i < size - 1; i++)
    {
      int j = i + rand() / (RAND_MAX / (size - i) + 1);
      if (j == i)
      {
        continue;
      }
      int int_j = array[j];

      array[j] = array[i];

      array[i] = int_j;
    }
  }
}

void allocFail(char *struct_name)
{
  printf("%s nao poude ser alocado\n", struct_name);
  printf("Encerrando o programa...\n");
  exit(1);
}

void shuffleDeck(Deck *deck)
{
  if (deck->deck_size > 1)
  {
    int i;
    for (i = 0; i < deck->deck_size - 1; i++)
    {
      int j = i + rand() / (RAND_MAX / (deck->deck_size - i) + 1);
      if (j == i)
      {
        continue;
      }
      Card *card_j = deck->cards[j];

      deck->cards[j] = deck->cards[i];

      deck->cards[i] = card_j;
    }
  }
}
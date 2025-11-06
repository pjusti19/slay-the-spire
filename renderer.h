#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

typedef struct
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_BITMAP *display_buffer;

  ALLEGRO_FONT *font;

} Renderer;

typedef enum
{
  WEAK = 1,
  STRONG = 2,
  //  TITAN = 3 -> implementar ao final do tp
} EnemyType;

typedef struct
{
  int healthbar;
  // Deck deck; -> implementar quando for mexer com o deck
} Player;

typedef struct
{
  int healthbar;
  EnemyType type;
} Enemy;

void FillRenderer(Renderer *renderer);

void Render(Renderer *renderer, Player *player);

void ClearRenderer(Renderer *renderer, Player *player);

#endif
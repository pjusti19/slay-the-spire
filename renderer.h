#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "player.h"
#include "enemy.h"
#include "deck.h"

typedef struct
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_BITMAP *display_buffer;

  ALLEGRO_FONT *font;

} Renderer;

void FillRenderer(Renderer *renderer);

void Render(Renderer *renderer, Player *player);

void ClearRenderer(Renderer *renderer, Player *player);

#endif
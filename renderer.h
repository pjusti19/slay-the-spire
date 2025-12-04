#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "player.h"
#include "enemy.h"
#include "deck.h"
#include "combat.h"

typedef struct
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_BITMAP *display_buffer;
  Combat *combat;
  ALLEGRO_FONT *font;

  ALLEGRO_BITMAP *grass_field_background;
  ALLEGRO_BITMAP *divine_beast_background;
  ALLEGRO_BITMAP *hyrule_castle_background;
  ALLEGRO_BITMAP *defense_card;
  ALLEGRO_BITMAP *attack_card;
  ALLEGRO_BITMAP *poison_card;
  ALLEGRO_BITMAP *vulnerability_card;
  ALLEGRO_BITMAP *weakness_card;
  ALLEGRO_BITMAP *lifesteal_card;
  ALLEGRO_BITMAP *strength_card;
  ALLEGRO_BITMAP *dexterity_card;
  ALLEGRO_BITMAP *special_card;
  ALLEGRO_BITMAP *kaioken_card;
  ALLEGRO_BITMAP *charge_card;

  ALLEGRO_BITMAP *stack_top;
  ALLEGRO_BITMAP *discard_stack_top;
  ALLEGRO_BITMAP *stamina_wheel;
  ALLEGRO_BITMAP *charge_gauge;

  ALLEGRO_BITMAP *player_frames[PLAYER_IDLE_FRAMES];
  int player_idle_frame_count;
  int player_idle_current_frame;
  int player_idle_frame_timer;
  int player_idle_frame_speed; 

  int actual_floor;
} Renderer;

void FillRenderer(Renderer *renderer);

void Render(Renderer *renderer);

void ClearRenderer(Renderer *renderer);

#endif
#include "renderer.h"

#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "utils.h"

void DrawScaledText(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y,
                    float xscale, float yscale, int alignment,
                    const char *text)
{
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform); // Start with an identity transform
  al_scale_transform(
      &transform, xscale,
      yscale);                  // Apply scaling (e.g., sx=2.0, sy=2.0 for double size)
  al_use_transform(&transform); // Use the transform for subsequent drawing

  al_draw_text(font, color, x, y, alignment, text);
  // al_draw_text(font, color, x, y, alignment, text);
  al_identity_transform(&transform);
  al_use_transform(&transform); // Use the transform for subsequent drawing
}

void DrawCenteredScaledText(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x,
                            float y, float xscale, float yscale, char *text)
{
  DrawScaledText(font, color, x, y, xscale, yscale, ALLEGRO_ALIGN_CENTRE, text);
}

void FillRenderer(Renderer *renderer)
{
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  renderer->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  must_init(renderer->display, "display");

  renderer->display_buffer =
      al_create_bitmap(DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT);
  must_init(renderer->display_buffer, "display buffer");

  renderer->font = al_create_builtin_font();
  must_init(renderer->font, "font");
}

void RenderBackground(Renderer *renderer)
{
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

void RenderDeck(Renderer *renderer, int x_left, int y_top, Deck *deck, ALLEGRO_FONT *font)
{
  ALLEGRO_BITMAP *prev_bmp_target = al_get_target_bitmap();

  ALLEGRO_BITMAP *deck_bitmap = al_create_bitmap(DECK_WIDTH, DECK_HEIGHT);
  al_set_target_bitmap(deck_bitmap);

  al_draw_filled_rounded_rectangle(0, 0, DECK_WIDTH, DECK_HEIGHT, 10, 0,
                                   al_map_rgb(255, 255, 255));

  float xscale = 1, yscale = 1;
  char text[100] = "";
  sprintf(text, "Cards: %d", deck->deck_size);
  DrawScaledText(renderer->font, al_map_rgb(0, 0, 0), (DECK_WIDTH * 0.5) / xscale,
                 (DECK_HEIGHT * 0.5) / yscale, xscale, yscale,
                 ALLEGRO_ALIGN_CENTER, text);

  al_set_target_bitmap(prev_bmp_target);

  al_draw_scaled_bitmap(deck_bitmap, 0, 0, DECK_WIDTH, DECK_HEIGHT, x_left,
                        y_top, DECK_WIDTH, DECK_HEIGHT, 0);
  al_destroy_bitmap(deck_bitmap);
}

void RenderHealthBar(int healthbar, float x_begin, float x_end, float y_down_left,
                     ALLEGRO_FONT *font)
{
  float mid_y = y_down_left - (HEALTH_BAR_HEIGHT * 0.78);

  al_draw_filled_rounded_rectangle(
      x_begin - HEALTH_BAR_BACKGROUND_EXTRA,
      y_down_left - HEALTH_BAR_BACKGROUND_EXTRA,
      x_end + HEALTH_BAR_BACKGROUND_EXTRA,
      y_down_left - HEALTH_BAR_HEIGHT + HEALTH_BAR_BACKGROUND_EXTRA,
      HEALTH_BAR_RX, HEALTH_BAR_RY, al_map_rgb(255, 255, 255));
  char text[100] = "";
  sprintf(text, "%d", healthbar);
  float x_scale = 2.0, y_scale = 2.0;
  DrawScaledText(font, al_map_rgb(0, 0, 0), (x_begin + x_end) / 2.0 / x_scale,
                 mid_y / y_scale, x_scale, y_scale, ALLEGRO_ALIGN_CENTRE, text);
}

void RenderPlayer(Player *player, const Renderer *renderer, int begin_x, int mid_y,
                  int width)
{
  al_draw_filled_circle(begin_x + width / 2.0, mid_y, width,
                        al_map_rgb(255, 255, 255));
  float x_end = begin_x + width;

  float health_bar_y = mid_y + width + 20;
  RenderHealthBar(player->player_stats->healthbar, begin_x, x_end, health_bar_y, renderer->font);

  // Suggestion: render enemy intents here.
}

void RenderCard(const Renderer *renderer, int x_left, int y_top, Card *card)
{
  ALLEGRO_BITMAP *card_bitmap = al_create_bitmap(CARD_WIDTH, CARD_HEIGHT);
  al_set_target_bitmap(card_bitmap);

  al_draw_filled_rounded_rectangle(0, 0, CARD_WIDTH, CARD_HEIGHT, 5, 5,
                                   al_map_rgb(255, 255, 255));
  al_draw_rounded_rectangle(0, 0, CARD_WIDTH, CARD_HEIGHT, 5, 5,
                            al_map_rgb(255, 0, 0), 2);

  char card_type[10] = "";
  switch (card->card_type)
  {
  case DEFENSE:
    sprintf(card_type, "Defesa");
  case ATTACK:
    sprintf(card_type, "Ataque");
  case SPECIAL:
    sprintf(card_type, "Especial");
  }

  char effect_rate[10] = "";
  sprintf(effect_rate, "%d", card->effect_rate);

  char cost[10] = "";
  sprintf(cost, "%d", card->cost);

  float xscale = 1, yscale = 1;
  ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);
  float font_height = al_get_font_line_height(renderer->font) * yscale;

  DrawScaledText(renderer->font, color, (CARD_WIDTH * 0.5) / xscale,
                 (CARD_HEIGHT * 0.3) / yscale, xscale, yscale,
                 ALLEGRO_ALIGN_CENTER, card_type);
  DrawScaledText(renderer->font, color, (CARD_WIDTH * 0.5) / xscale,
                 (CARD_HEIGHT * 0.3) / yscale + font_height, xscale, yscale,
                 ALLEGRO_ALIGN_CENTER, effect_rate);
  DrawScaledText(renderer->font, color, (CARD_WIDTH * 0.5) / xscale,
                 (CARD_HEIGHT * 0.3) / yscale + font_height * 2, xscale, yscale,
                 ALLEGRO_ALIGN_CENTER, cost);

  al_set_target_bitmap(renderer->display_buffer);
  al_draw_scaled_bitmap(card_bitmap, 0, 0, CARD_WIDTH, CARD_HEIGHT, x_left,
                        y_top, CARD_WIDTH, CARD_HEIGHT, 0);

  al_destroy_bitmap(card_bitmap);
}

void RenderPlayerHand(Renderer *renderer, Player *player)
{
  int card_number;
  for (int i = 0; i < player->hand->deck_size; i++)
  {
    card_number = rand() % 20;
    RenderCard(renderer, HAND_BEGIN_X + i * 120, HAND_BEGIN_Y, player->deck->cards[card_number]);
    discardCard(player->deck, card_number);
  }
}

void RenderEnemies(Renderer *renderer) {}

void RenderEnergy(Renderer *renderer, int energy, int begin_x, int mid_y, int width)
{
  al_draw_filled_circle(begin_x + width / 2.0, mid_y, width,
                        al_map_rgb(255, 251, 0));

  char energy_gauge[10] = "";
  sprintf(energy_gauge, "%d/3", energy);

  float xscale = 1.7, yscale = 1.7;
  ALLEGRO_COLOR color = al_map_rgb(0, 0, 0);
  DrawScaledText(renderer->font, color, ENERGY_GAUGE_BEGIN_X / xscale,
                 ENERGY_GAUGE_BEGIN_Y / yscale, xscale, yscale,
                 ALLEGRO_ALIGN_LEFT, energy_gauge);
}

void Render(Renderer *renderer, Combat *combat)
{
  al_set_target_bitmap(renderer->display_buffer);
  RenderBackground(renderer);
  // if (combat->player->deck->deck_size > 0)
  RenderDeck(renderer, DRAW_DECK_X, DRAW_DECK_Y, combat->player->deck, renderer->font);
  RenderPlayer(combat->player, renderer, PLAYER_BEGIN_X, PLAYER_BEGIN_Y + PLAYER_RADIUS,
               PLAYER_RADIUS);
  RenderEnergy(renderer, combat->player->energy, ENERGY_GAUGE_BEGIN_X, ENERGY_GAUGE_BEGIN_Y, ENERGY_GAUGE_RADIUS);
  RenderEnemies(renderer);
  RenderPlayerHand(renderer, combat->player);
  al_set_target_backbuffer(renderer->display);

  al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH,
                        DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH,
                        DISPLAY_HEIGHT, 0);

  al_flip_display();
}

void ClearRenderer(Renderer *renderer, Combat *combat)
{
  freePlayer(combat->player);
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);
  al_destroy_font(renderer->font);
}
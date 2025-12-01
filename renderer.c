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

void DrawMultilineText(ALLEGRO_FONT *font, ALLEGRO_COLOR color,
                       float x, float y, float line_height,
                       int align, const char *text)
{
  char line[256];
  int i = 0, j = 0;

  while (1)
  {
    if (text[i] == '\n' || text[i] == '\0')
    {
      line[j] = '\0';

      if (align == ALLEGRO_ALIGN_CENTER)
        al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTER, line);
      else if (align == ALLEGRO_ALIGN_RIGHT)
        al_draw_text(font, color, x, y, ALLEGRO_ALIGN_RIGHT, line);
      else
        al_draw_text(font, color, x, y, 0, line);

      y += line_height;
      j = 0;

      if (text[i] == '\0')
        break;

      i++;
      continue;
    }

    line[j++] = text[i++];
  }
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
  al_clear_to_color(COLOR_BLACK);
}

void RenderStack(Renderer *renderer, int x_left, int y_top, Deck *stack)
{
  ALLEGRO_BITMAP *prev_bmp_target = al_get_target_bitmap();

  ALLEGRO_BITMAP *deck_bitmap = al_create_bitmap(DECK_WIDTH, DECK_HEIGHT);
  al_set_target_bitmap(deck_bitmap);

  al_draw_filled_rounded_rectangle(0, 0, DECK_WIDTH, DECK_HEIGHT, 10, 0,
                                   al_map_rgb(255, 255, 255));

  float xscale = 1, yscale = 1;
  char text[100] = "";
  sprintf(text, "Cards: %d", stack->deck_size);
  DrawScaledText(renderer->font, COLOR_BLACK, (DECK_WIDTH * 0.5) / xscale,
                 (DECK_HEIGHT * 0.5) / yscale, xscale, yscale,
                 ALLEGRO_ALIGN_CENTER, text);

  al_set_target_bitmap(prev_bmp_target);

  al_draw_scaled_bitmap(deck_bitmap, 0, 0, DECK_WIDTH, DECK_HEIGHT, x_left,
                        y_top, DECK_WIDTH, DECK_HEIGHT, 0);
  al_destroy_bitmap(deck_bitmap);
}

void RenderHealthBar(Stats *stats, float x_begin, float x_end, float y_down_left,
                     ALLEGRO_FONT *font)
{
  y_down_left += 5;

  float mid_y = y_down_left - (HEALTH_BAR_HEIGHT * 0.78);

  float total_width = x_end - x_begin;
  float healthbar_percentage = (float)stats->healthbar / (float)stats->max_health;
  float filled_width = total_width * healthbar_percentage;

  if (stats->shieldbar > 0)
  {
    float shield_radius = 10;
    float shield_x = x_end + 20;
    float shield_y = y_down_left - (HEALTH_BAR_HEIGHT / 2);

    al_draw_filled_circle(shield_x, shield_y, shield_radius, al_map_rgb(0, 150, 255));
    al_draw_circle(shield_x, shield_y, shield_radius, al_map_rgb(255, 255, 255), 2);

    char shield_text[16];
    sprintf(shield_text, "%d", stats->shieldbar);

    float scale = 0.9;

    DrawScaledText(font, COLOR_WHITE, shield_x / scale + 1, shield_y / scale - 3, scale, scale, ALLEGRO_ALIGN_CENTRE, shield_text);
  }

  ALLEGRO_COLOR healthbar_color = COLOR_DARK_RED;
  if (stats->poison > 0)
    healthbar_color = COLOR_GREY_GREEN;

  al_draw_filled_rounded_rectangle(
      x_begin - HEALTH_BAR_BACKGROUND_EXTRA + 3,
      y_down_left - HEALTH_BAR_BACKGROUND_EXTRA - 6,
      x_begin - HEALTH_BAR_BACKGROUND_EXTRA - 2 + filled_width,
      y_down_left - HEALTH_BAR_HEIGHT + HEALTH_BAR_BACKGROUND_EXTRA + 3,
      HEALTH_BAR_RX, HEALTH_BAR_RY, healthbar_color);

  al_draw_rounded_rectangle(
      x_begin - HEALTH_BAR_BACKGROUND_EXTRA,
      y_down_left - HEALTH_BAR_BACKGROUND_EXTRA - 3,
      x_end + HEALTH_BAR_BACKGROUND_EXTRA,
      y_down_left - HEALTH_BAR_HEIGHT + HEALTH_BAR_BACKGROUND_EXTRA,
      HEALTH_BAR_RX, HEALTH_BAR_RY, al_map_rgb(163, 163, 163), 2);

  char text[100] = "";
  sprintf(text, "%d/%d", stats->healthbar, stats->max_health);
  float x_scale = 1.2, y_scale = 1.2;
  DrawScaledText(font, COLOR_WHITE, (x_begin + x_end) / 2.0 / x_scale,
                 mid_y / y_scale, x_scale, y_scale, ALLEGRO_ALIGN_CENTRE, text);
}

void RenderPlayer(const Renderer *renderer, int begin_x, int mid_y, int width)
{
  float pr = width;
  float px = begin_x + pr;
  float py = mid_y;

  al_draw_filled_circle(px, py, pr, al_map_rgb(255, 255, 255));

  if (renderer->combat->__has_passive_card_been_used)
  {
    float tip_x = px;
    float tip_y = py - pr - 10;

    al_draw_filled_triangle(
        tip_x, tip_y,
        tip_x - 20, tip_y - 30,
        tip_x + 20, tip_y - 30,
        al_map_rgb(255, 0, 0));
  }

  float bar_width = (pr * 2) * 0.9f;

  float x_begin = px - bar_width / 2.0f;
  float x_end = px + bar_width / 2.0f;

  float health_bar_y = py + pr + 25;

  RenderHealthBar(renderer->combat->player->player_stats,
                  x_begin, x_end, health_bar_y,
                  renderer->font);
}

void RenderCard(const Renderer *renderer, int x_left, int y_top, Card *card, bool __is_card_pointed)
{
  ALLEGRO_BITMAP *card_bitmap = al_create_bitmap(CARD_WIDTH + 10, CARD_HEIGHT);
  al_set_target_bitmap(card_bitmap);

  al_draw_filled_rounded_rectangle(10, 10, CARD_WIDTH - 10, CARD_HEIGHT - 10, 5, 5, al_map_rgb(255, 255, 255));
  al_draw_rounded_rectangle(10, 10, CARD_WIDTH - 10, CARD_HEIGHT - 10, 5, 5, al_map_rgb(255, 0, 0), 2);

  char card_type[20];
  char effect_rate[64];
  char cost[20];

  sprintf(cost, "Custo: %d", card->cost);

  switch (card->card_type)
  {
  case DEFENSE:
    sprintf(card_type, "Defesa");
    sprintf(effect_rate, "Escudo: %d", card->effect_rate);
    break;

  case ATTACK:
    sprintf(card_type, "Ataque");
    sprintf(effect_rate, "Dano: %d", card->effect_rate);
    break;

  case SPECIAL:
    sprintf(card_type, "Especial");
    sprintf(effect_rate, "");
    break;

  case LIFESTEAL:
    sprintf(card_type, "Vampirismo");
    sprintf(effect_rate, "Cura: %d%%\n"
                         "do ataque",
            card->effect_rate);
    break;
  case STRENGTH:
    sprintf(card_type, "Força");
    sprintf(effect_rate, "+ %d de\n"
                         "dano extra",
            card->effect_rate);
    break;
  case DEXTERITY:
    sprintf(card_type, "Destreza");
    sprintf(effect_rate, "+ %d de\n"
                         "escudo extra",
            card->effect_rate);
    break;
  case VULNERABILITY:
    sprintf(card_type, "Vulnerabilidade");
    sprintf(effect_rate, "%dx + dano\n"
                         "no inimigo",
            card->effect_rate);
    break;
  case WEAKNESS:
    sprintf(card_type, "Fraqueza");
    sprintf(effect_rate, "%dx - dano\n"
                         "do inimigo",
            card->effect_rate);
    break;
  case POISON:
    sprintf(card_type, "Veneno");
    sprintf(effect_rate, "Base: %d",
            card->effect_rate);
    break;
  }

  float xscale = 1, yscale = 1;
  float font_height = al_get_font_line_height(renderer->font);

  DrawScaledText(renderer->font, COLOR_BLACK, CARD_WIDTH * 0.5,
                 CARD_HEIGHT * 0.3, 1, 1, ALLEGRO_ALIGN_CENTER, card_type);

  if (card->card_type != ATTACK && card->card_type != DEFENSE && card->card_type != SPECIAL)
    DrawMultilineText(renderer->font, COLOR_BLACK,
                      CARD_WIDTH * 0.5,
                      CARD_HEIGHT * 0.4 + font_height,
                      font_height,
                      ALLEGRO_ALIGN_CENTER,
                      effect_rate);
  else
    DrawScaledText(renderer->font, COLOR_BLACK, CARD_WIDTH * 0.5,
                   CARD_HEIGHT * 0.4 + font_height,
                   1, 1, ALLEGRO_ALIGN_CENTER, effect_rate);

  DrawScaledText(renderer->font, COLOR_BLACK, CARD_WIDTH * 0.5,
                 CARD_HEIGHT * 0.5 + font_height * 3,
                 1, 1, ALLEGRO_ALIGN_CENTER, cost);

  if (__is_card_pointed)
    y_top -= 70;

  al_set_target_bitmap(renderer->display_buffer);
  al_draw_scaled_bitmap(card_bitmap, 0, 0, CARD_WIDTH, CARD_HEIGHT,
                        x_left, y_top, CARD_WIDTH, CARD_HEIGHT, 0);

  al_destroy_bitmap(card_bitmap);
}

void RenderPlayerHand(Renderer *renderer)
{

  for (int i = 0; i < renderer->combat->player->hand->deck_size; i++)
  {
    bool __is_card_pointed = false;
    if (renderer->combat->pointed_card == i)
      __is_card_pointed = true;
    RenderCard(renderer, HAND_BEGIN_X + i * 120 + (60 * -(renderer->combat->player->hand->deck_size - DEFAULT_HAND_STACK)), HAND_BEGIN_Y, renderer->combat->player->hand->cards[i], __is_card_pointed);
  }
}

void RenderEnergy(Renderer *renderer, int begin_x, int mid_y, int width)
{
  al_draw_filled_circle(begin_x + width / 2.0, mid_y, width, al_map_rgb(255, 251, 0));

  char energy_gauge[10] = "";
  sprintf(energy_gauge, "%d/3", renderer->combat->player->energy);

  float xscale = 1.7, yscale = 1.7;
  DrawScaledText(renderer->font, COLOR_BLACK, ENERGY_GAUGE_BEGIN_X / xscale, ENERGY_GAUGE_BEGIN_Y / yscale, xscale, yscale, ALLEGRO_ALIGN_LEFT, energy_gauge);
}

void RenderEnemies(Renderer *renderer, int x_left, int y_top, int actual_enemy)
{
  Enemy *enemy = renderer->combat->enemy_group->enemies[actual_enemy];

  // Adjustment for distinction of enemy type
  int enemy_height = ENEMY_HEIGHT;
  if (enemy->enemy_type == 2)
    enemy_height += 70;

  if (renderer->combat->pointed_enemy == actual_enemy && renderer->combat->__has_enter_been_pressed == true)
  {
    al_draw_filled_triangle(
        x_left + ENEMY_WIDTH / 2.0, y_top - 10,      // ponta (embaixo, mais perto do inimigo)
        x_left + ENEMY_WIDTH / 2.0 - 20, y_top - 40, // base esquerda (em cima)
        x_left + ENEMY_WIDTH / 2.0 + 20, y_top - 40, // base direita (em cima)
        al_map_rgb(255, 0, 0));
  }

  ALLEGRO_BITMAP *prev_bmp_target = al_get_target_bitmap();

  ALLEGRO_BITMAP *enemy_bitmap = al_create_bitmap(ENEMY_WIDTH, enemy_height);
  al_set_target_bitmap(enemy_bitmap);

  al_draw_filled_rounded_rectangle(0, 0, ENEMY_WIDTH, enemy_height, 5, 5, al_map_rgb(255, 255, 255));
  al_draw_rounded_rectangle(0, 0, ENEMY_WIDTH, enemy_height, 5, 5, al_map_rgb(255, 0, 0), 2);

  int actual_action = enemy->actual_action;
  ALLEGRO_COLOR color;
  char action_type[10] = "";
  switch (enemy->actions->cards[actual_action]->card_type)
  {
  case ATTACK:
    sprintf(action_type, "ATK");
    color = COLOR_RED;
    break;
  case DEFENSE:
    sprintf(action_type, "DEF");
    color = COLOR_AQUA_BLUE;
    break;
  }

  al_set_target_bitmap(prev_bmp_target);

  al_draw_scaled_bitmap(enemy_bitmap, 0, 0, ENEMY_WIDTH, enemy_height, x_left, y_top, ENEMY_WIDTH, enemy_height, 0);

  float text_x = x_left + ENEMY_WIDTH - 5; // canto superior direito
  float text_y = y_top - 10;

  DrawScaledText(renderer->font, color,
                 text_x, text_y,
                 1.0, 1.0,
                 ALLEGRO_ALIGN_RIGHT,
                 action_type);

  al_destroy_bitmap(enemy_bitmap);

  float x_end = x_left + ENEMY_WIDTH - 3;
  float health_bar_y = y_top + enemy_height + 20;
  RenderHealthBar(enemy->enemy_stats, x_left + 3, x_end, health_bar_y, renderer->font);
}

void Render(Renderer *renderer)
{
  al_set_target_bitmap(renderer->display_buffer);

  RenderBackground(renderer);

  if (renderer->combat->player->stack->deck_size > 0)
    RenderStack(renderer, DRAW_DECK_X, DRAW_DECK_Y, renderer->combat->player->stack);

  if (renderer->combat->player->discard_stack->deck_size > 0)
    RenderStack(renderer, DRAW_DECK_X + 815, DRAW_DECK_Y, renderer->combat->player->discard_stack);

  RenderPlayer(renderer, PLAYER_BEGIN_X, PLAYER_BEGIN_Y + PLAYER_RADIUS, PLAYER_RADIUS);

  RenderEnergy(renderer, ENERGY_GAUGE_BEGIN_X, ENERGY_GAUGE_BEGIN_Y, ENERGY_GAUGE_RADIUS);

  for (int i = 0; i < renderer->combat->enemy_group->enemy_amount; i++)
  {
    if (renderer->combat->enemy_group->enemies[i]->enemy_stats->healthbar > 0)
      RenderEnemies(renderer, ENEMY_BEGIN_X + i * 150, ENEMY_BEGIN_Y + i * 50, i);
  }

  RenderPlayerHand(renderer);

  al_set_target_backbuffer(renderer->display);

  al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

  al_flip_display();
}

void ClearRenderer(Renderer *renderer)
{
  freeCombat(renderer->combat);
  freePlayer(renderer->combat->player);
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);
  al_destroy_font(renderer->font);
}
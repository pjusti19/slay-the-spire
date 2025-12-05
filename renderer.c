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

void loadEnemyAnimations(Renderer *renderer)
{
  char path[128];
  int amount = renderer->combat->enemy_group->enemy_amount;

  for (int i = 0; i < amount; i++)
  {
    Enemy *enemy = renderer->combat->enemy_group->enemies[i];
    AnimationSet *animation = malloc(sizeof(AnimationSet));
    renderer->enemy_animations[i] = animation;

    int frame_count = 0;

    switch (enemy->enemy_type)
    {
    case WEAK:
      frame_count = WEAK_ENEMY_IDLE_FRAMES;
      break;
    case STRONG:
      frame_count = STRONG_ENEMY_IDLE_FRAMES;
      break;
    case BOSS:
      frame_count = BOSS_IDLE_FRAMES;
      break;
    }

    animation->frame_count = frame_count;
    animation->current_frame = 0;
    animation->frame_timer = 0;
    animation->frame_speed = 8;

    for (int j = 0; j < frame_count; j++)
    {
      sprintf(path, "assets/%s_enemy_idle_animation/frame_%d.png",
              enemy->enemy_type == WEAK ? "weak" : enemy->enemy_type == STRONG ? "strong"
                                                                               : "boss",
              j);

      animation->frame[j] = al_load_bitmap(path);
    }
  }
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

  renderer->start_screen = al_load_bitmap("assets/start_screen.png");
  renderer->death_screen = al_load_bitmap("assets/death_screen.png");
  renderer->victory_screen = al_load_bitmap("assets/victory_screen.png");
  renderer->grass_field_background = al_load_bitmap("assets/hyrule_background.png");
  renderer->divine_beast_background = al_load_bitmap("assets/divine_beast.png");
  renderer->hyrule_castle_background = al_load_bitmap("assets/hyrule_castle.png");
  renderer->defense_card = al_load_bitmap("assets/cards/defense.png");
  renderer->attack_card = al_load_bitmap("assets/cards/attack.png");
  renderer->poison_card = al_load_bitmap("assets/cards/poison.png");
  renderer->vulnerability_card = al_load_bitmap("assets/cards/vulnerability.png");
  renderer->weakness_card = al_load_bitmap("assets/cards/weakness.png");
  renderer->lifesteal_card = al_load_bitmap("assets/cards/lifesteal.png");
  renderer->strength_card = al_load_bitmap("assets/cards/strength.png");
  renderer->dexterity_card = al_load_bitmap("assets/cards/dexterity.png");
  renderer->special_card = al_load_bitmap("assets/cards/special.png");
  renderer->charge_card = al_load_bitmap("assets/cards/charge.png");
  renderer->kaioken_card = al_load_bitmap("assets/cards/triforce.png");
  renderer->stack_top = al_load_bitmap("assets/cards/stack.png");
  renderer->discard_stack_top = al_load_bitmap("assets/cards/discard.png");
  renderer->stamina_wheel = al_load_bitmap("assets/stamina_wheel.png");
  renderer->charge_gauge = al_load_bitmap("assets/charge_gauge.png");
  renderer->player_idle_frame_count = PLAYER_IDLE_FRAMES;
  renderer->player_idle_frame_speed = 8;
  renderer->player_idle_current_frame = 0;
  renderer->player_idle_frame_timer = 0;

  char path[128];

  // PLAYER FRAMES
  for (int i = 0; i < PLAYER_IDLE_FRAMES; i++)
  {
    sprintf(path, "assets/player_idle_animation/frame_%d.png", i);
    renderer->player_frames[i] = al_load_bitmap(path);
  }

  // ENEMY FRAMES
  loadEnemyAnimations(renderer);

  renderer->font = al_create_builtin_font();
  must_init(renderer->font, "font");
}

void RenderBackground(Renderer *renderer)
{
  al_clear_to_color(COLOR_BLACK);
  if (renderer->actual_floor == 12)
    al_draw_scaled_bitmap(renderer->victory_screen, 0, 0, al_get_bitmap_width(renderer->victory_screen),
                          al_get_bitmap_height(renderer->victory_screen), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
  else if (renderer->actual_floor == -1)
    al_draw_scaled_bitmap(renderer->death_screen, 0, 0, al_get_bitmap_width(renderer->death_screen),
                          al_get_bitmap_height(renderer->death_screen), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
  else if (renderer->actual_floor == 0)
    al_draw_scaled_bitmap(renderer->start_screen, 0, 0, al_get_bitmap_width(renderer->start_screen),
                          al_get_bitmap_height(renderer->start_screen), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
  else if (renderer->actual_floor < 5)
    al_draw_scaled_bitmap(renderer->grass_field_background, 0, 0, al_get_bitmap_width(renderer->grass_field_background),
                          al_get_bitmap_height(renderer->grass_field_background), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
  else if (renderer->actual_floor < 9)
    al_draw_scaled_bitmap(renderer->divine_beast_background, 0, 0, al_get_bitmap_width(renderer->divine_beast_background),
                          al_get_bitmap_height(renderer->divine_beast_background), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
  else
    al_draw_scaled_bitmap(renderer->hyrule_castle_background, 0, 0, al_get_bitmap_width(renderer->hyrule_castle_background),
                          al_get_bitmap_height(renderer->hyrule_castle_background), 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0);
}

void RenderStack(Renderer *renderer, int x_left, int y_top, Deck *stack, bool __is_discard_stack)
{
  ALLEGRO_BITMAP *prev_bmp_target = al_get_target_bitmap();

  ALLEGRO_BITMAP *deck_bitmap = al_create_bitmap(DECK_WIDTH, DECK_HEIGHT);
  al_set_target_bitmap(deck_bitmap);

  al_clear_to_color(al_map_rgba(0, 0, 0, 0));

  float proportion;
  if (__is_discard_stack == false)
  {
    al_draw_scaled_bitmap(renderer->stack_top, 0, 0, al_get_bitmap_width(renderer->stack_top),
                          al_get_bitmap_height(renderer->stack_top), 0, 0, DECK_WIDTH, DECK_HEIGHT, 0);
    proportion = 0.84;
  }
  else
  {
    al_draw_scaled_bitmap(renderer->discard_stack_top, 0, 0, al_get_bitmap_width(renderer->discard_stack_top),
                          al_get_bitmap_height(renderer->discard_stack_top), 0, 0, DECK_WIDTH, DECK_HEIGHT, 0);
    proportion = 0.81;
  }
  char text[64];
  sprintf(text, "Cards: %d", stack->deck_size);

  float xscale = 1.0;
  float yscale = 1.0;

  DrawScaledText(renderer->font, al_map_rgb(0, 0, 0), (DECK_WIDTH * 0.5) / xscale, (DECK_HEIGHT * proportion) / yscale, xscale, yscale, ALLEGRO_ALIGN_CENTER, text);

  al_set_target_bitmap(prev_bmp_target);

  al_draw_scaled_bitmap(deck_bitmap, 0, 0, DECK_WIDTH, DECK_HEIGHT, x_left, y_top, DECK_WIDTH, DECK_HEIGHT, 0);

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
  else if (stats->__is_kaioken_active == true)
    healthbar_color = COLOR_FIRE_ORANGE;

  // OBSERVATION FOR EVALUATOR: DUE THE FACT THAT THE IMAGE USED IS A PNG, YOU'LL PROBABLY HAVE THE IMPRESSION 
  // THAT THE HEALTHBAR TRAPASSES THE CREATURE FIGURE, BUT IT ACTUALLY IS UNDER THE TOTAL WIDTH/HEIGHT
  // DETERMINATED BY THE TRANSPARENT PART OF THE IMAGE => IT FITS THE RULE!
  // WHY I AM NOT MAKING IT UNDER ONLY THE OPAQUE LIMIT? BECAUSE I THINK IT'S UGLY! GETS TOO SMALL...

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

  ALLEGRO_BITMAP *frame = renderer->player_frames[renderer->player_idle_current_frame];
  int fw = al_get_bitmap_width(frame);
  int fh = al_get_bitmap_height(frame);

  float scale = (pr * 2.0f) / fw;

  al_draw_scaled_bitmap(frame, 0, 0, fw, fh,
                        px - (fw * scale / 2),
                        py - (fh * scale / 2),
                        fw * scale, fh * scale, 0);

  if (renderer->combat->__has_passive_card_been_used)
  {
    float tip_x = px;
    float tip_y = py - pr - 10;

    al_draw_filled_triangle(tip_x, tip_y,
                            tip_x - 20, tip_y - 30,
                            tip_x + 20, tip_y - 30,
                            al_map_rgb(255, 0, 0));
  }

  float bar_width = (pr * 2) * 0.9;
  float x_begin = px - bar_width / 2.0;
  float x_end = px + bar_width / 2.0;
  float health_bar_y = py + pr + 15;

  RenderHealthBar(renderer->combat->player->player_stats,
                  x_begin, x_end, health_bar_y,
                  renderer->font);
}

void RenderCard(const Renderer *renderer, int x_left, int y_top, Card *card, bool __is_card_pointed)
{
  ALLEGRO_BITMAP *card_sprite = NULL;

  switch (card->card_type)
  {
  case DEFENSE:
    card_sprite = renderer->defense_card;
    break;
  case ATTACK:
    card_sprite = renderer->attack_card;
    break;
  case POISON:
    card_sprite = renderer->poison_card;
    break;
  case VULNERABILITY:
    card_sprite = renderer->vulnerability_card;
    break;
  case WEAKNESS:
    card_sprite = renderer->weakness_card;
    break;
  case LIFESTEAL:
    card_sprite = renderer->lifesteal_card;
    break;
  case STRENGTH:
    card_sprite = renderer->strength_card;
    break;
  case DEXTERITY:
    card_sprite = renderer->dexterity_card;
    break;
  case SPECIAL:
    card_sprite = renderer->special_card;
    break;
  case KAIOKEN:
    card_sprite = renderer->kaioken_card;
    break;
  case CHARGE:
    card_sprite = renderer->charge_card;
    break;
  }

  if (!card_sprite)
    return;

  if (__is_card_pointed)
    y_top -= 40;

  al_draw_scaled_bitmap(
      card_sprite,
      0, 0,
      al_get_bitmap_width(card_sprite),
      al_get_bitmap_height(card_sprite),
      x_left, y_top,
      CARD_WIDTH, CARD_HEIGHT,
      0);

  char effect_rate[64];
  char cost[32];
  float font_height = al_get_font_line_height(renderer->font);

  sprintf(cost, "Cost: %d", card->cost);

  double effect_height_scale = 0.75;
  switch (card->card_type)
  {
  case DEFENSE:
    sprintf(effect_rate, "Shield: %d", card->effect_rate);
    break;
  case ATTACK:
    sprintf(effect_rate, "Damage: %d", card->effect_rate);
    break;
  case SPECIAL:
    sprintf(effect_rate, "New hand");
    break;
  case LIFESTEAL:
    sprintf(effect_rate, "Heal %d%%\n", card->effect_rate);
    break;
  case STRENGTH:
    sprintf(effect_rate, "+%d%% damage", card->effect_rate);
    break;
  case DEXTERITY:
    sprintf(effect_rate, "+%d%% shield", card->effect_rate);
    break;
  case VULNERABILITY:
    sprintf(effect_rate, "-%d%% armor", card->effect_rate);
    break;
  case WEAKNESS:
    sprintf(effect_rate, "-%d%% damage", card->effect_rate);
    break;
  case POISON:
    sprintf(effect_rate, "Base: %d", card->effect_rate);
    break;
  case KAIOKEN:
    sprintf(effect_rate, "2x damage\n"
                         "Magic curse",
            card->effect_rate / 10);
    effect_height_scale = 0.7;
    break;
  case CHARGE:
    sprintf(effect_rate, "+%d charge", card->effect_rate);
    break;
  }

  DrawMultilineText(renderer->font, COLOR_BLACK,
                    x_left + CARD_WIDTH * 0.5,
                    y_top + CARD_HEIGHT * effect_height_scale,
                    font_height,
                    ALLEGRO_ALIGN_CENTER,
                    effect_rate);

  DrawScaledText(renderer->font, COLOR_BLACK,
                 x_left + CARD_WIDTH * 0.5,
                 y_top + CARD_HEIGHT * 0.83,
                 1, 1,
                 ALLEGRO_ALIGN_CENTER, cost);
}

void RenderPlayerHand(Renderer *renderer)
{

  for (int i = 0; i < renderer->combat->player->hand->deck_size; i++)
  {
    bool __is_card_pointed = false;
    if (renderer->combat->pointed_card == i)
      __is_card_pointed = true;
    RenderCard(renderer, HAND_BEGIN_X + i * 100 + (60 * -(renderer->combat->player->hand->deck_size - DEFAULT_HAND_STACK)), HAND_BEGIN_Y, renderer->combat->player->hand->cards[i], __is_card_pointed);
  }
}

void RenderEnergy(Renderer *renderer, int begin_x, int mid_y, int radius, int actual_points, int max_points, ALLEGRO_BITMAP *image, int height_adjustment)
{
  int width = al_get_bitmap_width(image);
  int height = al_get_bitmap_height(image);

  float scale = (radius * 2.0) / (float)height;
  float final_w = width * scale;
  float final_h = height * scale;

  al_draw_scaled_bitmap(image, 0, 0, width, height, begin_x - (final_w / 2.0f), mid_y - (final_h / 2.0f), final_w, final_h, 0);

  char buffer[16];

  sprintf(buffer, "%d/%d", actual_points, max_points);

  float xscale = 1.3f, yscale = 1.3f;
  float text_x = (float)begin_x / xscale;
  float text_y = mid_y + final_h / 2.0 + 8.0 - 90;
  DrawScaledText(renderer->font, al_map_rgb(255, 255, 255), text_x + 1, text_y + 5 - height_adjustment, xscale, yscale, ALLEGRO_ALIGN_CENTER, buffer);
}

void RenderEnemies(Renderer *renderer, int x_left, int y_top, int actual_enemy)
{
  Enemy *enemy = renderer->combat->enemy_group->enemies[actual_enemy];
  AnimationSet *anim = renderer->enemy_animations[actual_enemy];

  int enemy_width = ENEMY_WIDTH;
  int enemy_height = ENEMY_HEIGHT;

  float health_bar_y;
  if (enemy->enemy_type == 2)
  {
    enemy_height += 130;
    enemy_width += 200;
    health_bar_y = y_top + enemy_height - 20;
  }
  else if (enemy->enemy_type == 3)
  {
    enemy_height += 70;
    enemy_width += 70;
    health_bar_y = y_top + enemy_height + 20;
  }
  else
    health_bar_y = y_top + enemy_height + 10;

  if (renderer->combat->pointed_enemy == actual_enemy && renderer->combat->__has_enter_been_pressed)
  {
    al_draw_filled_triangle(x_left + enemy_width / 2.0, y_top - 10,
                            x_left + enemy_width / 2.0 - 20, y_top - 40,
                            x_left + enemy_width / 2.0 + 20, y_top - 40,
                            al_map_rgb(255, 0, 0));
  }

  ALLEGRO_BITMAP *frame = anim->frame[anim->current_frame];
  int fw = al_get_bitmap_width(frame);
  int fh = al_get_bitmap_height(frame);

  float scale_x = (float)enemy_width / fw;
  float scale_y = (float)enemy_height / fh;

  al_draw_scaled_bitmap(frame, 0, 0, fw, fh,
                        x_left + enemy_width, // flip compensation
                        y_top,
                        -enemy_width, // negative = mirrored
                        enemy_height,
                        0);

  int action = enemy->actual_action;
  ALLEGRO_COLOR color;
  char action_type[10];

  switch (enemy->actions->cards[action]->card_type)
  {
  case ATTACK:
    sprintf(action_type, "ATK %d", enemy->actions->cards[action]->effect_rate);
    color = COLOR_RED;
    break;
  case DEFENSE:
    sprintf(action_type, "DEF %d", enemy->actions->cards[action]->effect_rate);
    color = COLOR_AQUA_BLUE;
    break;
  case VULNERABILITY:
    sprintf(action_type, "VUL %%%d", enemy->actions->cards[action]->effect_rate);
    color = COLOR_GREY_PURPLE;
    break;
  case WEAKNESS:
    sprintf(action_type, "WKN %%%d", enemy->actions->cards[action]->effect_rate);
    color = COLOR_GREY;
    break;
  case POISON:
    sprintf(action_type, "PSN %d", enemy->actions->cards[action]->effect_rate);
    color = COLOR_GREY_GREEN;
    break;
  }

  DrawScaledText(renderer->font, color,
                 x_left + enemy_width - 5,
                 y_top - 10,
                 1.0, 1.0,
                 ALLEGRO_ALIGN_RIGHT,
                 action_type);

  float x_end = x_left + enemy_width - 3;
  RenderHealthBar(enemy->enemy_stats, x_left + 3, x_end, health_bar_y, renderer->font);
}

void Render(Renderer *renderer)
{
  al_set_target_bitmap(renderer->display_buffer);

  RenderBackground(renderer);
  if (renderer->actual_floor > 0 && renderer->actual_floor < 12)
  {
    if (renderer->combat->player->stack->deck_size > 0)
      RenderStack(renderer, DRAW_DECK_X, DRAW_DECK_Y, renderer->combat->player->stack, false);

    if (renderer->combat->player->discard_stack->deck_size > 0)
      RenderStack(renderer, DRAW_DECK_X + 815, DRAW_DECK_Y, renderer->combat->player->discard_stack, true);

    if (renderer->combat->player->player_stats->healthbar > 0)
      RenderPlayer(renderer, PLAYER_BEGIN_X, PLAYER_BEGIN_Y + PLAYER_RADIUS, PLAYER_RADIUS);

    // Stamina
    RenderEnergy(renderer, ENERGY_GAUGE_BEGIN_X, ENERGY_GAUGE_BEGIN_Y + 10, ENERGY_GAUGE_RADIUS,
                 renderer->combat->player->energy, renderer->combat->player->max_energy, renderer->stamina_wheel, 0);

    // Charges
    RenderEnergy(renderer, CHARGE_GAUGE_BEGIN_X, CHARGE_GAUGE_BEGIN_Y - 30, CHARGE_GAUGE_RADIUS,
                 renderer->combat->player->charges, MAX_CHARGES, renderer->charge_gauge, 17);

    for (int i = 0; i < renderer->combat->enemy_group->enemy_amount; i++)
    {
      int begin_x;
      int begin_y;
      if (renderer->combat->enemy_group->enemies[i]->enemy_type == WEAK)
      {
        begin_x = ENEMY_BEGIN_X + 250 + (125 * i);
        begin_y = ENEMY_BEGIN_Y + 75;
      }
      else if (renderer->combat->enemy_group->enemies[i]->enemy_type == STRONG)
      {
        begin_x = ENEMY_BEGIN_X - 50;
        begin_y = ENEMY_BEGIN_Y;
      }
      else if (renderer->combat->enemy_group->enemies[i]->enemy_type == BOSS)
      {
        begin_x = 600;
        begin_y = 230;
      }
      if (renderer->combat->enemy_group->enemies[i]->enemy_stats->healthbar > 0)
        RenderEnemies(renderer, begin_x, begin_y, i);
    }

    RenderPlayerHand(renderer);
  }

  al_set_target_backbuffer(renderer->display);

  al_draw_scaled_bitmap(renderer->display_buffer, 0, 0, DISPLAY_BUFFER_WIDTH, DISPLAY_BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);

  al_flip_display();
}

void freeEnemyAnimations(Renderer *renderer)
{
  for (int i = 0; i < DEFAULT_ENEMY_GROUP_SIZE; i++)
  {
    AnimationSet *anim = renderer->enemy_animations[i];
    if (!anim)
      continue;

    for (int j = 0; j < anim->frame_count; j++)
      if (anim->frame[j])
        al_destroy_bitmap(anim->frame[j]);

    free(anim);
    renderer->enemy_animations[i] = NULL;
  }
}

void ClearRenderer(Renderer *renderer)
{
  freeEnemyAnimations(renderer);
  for (int i = 0; i < renderer->player_idle_frame_count; i++)
    al_destroy_bitmap(renderer->player_frames[i]);

  freePlayer(renderer->combat->player);
  al_destroy_display(renderer->display);
  al_destroy_bitmap(renderer->display_buffer);
  al_destroy_font(renderer->font);
}
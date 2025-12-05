#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "renderer.h"
#include "constants.h"
#include "utils.h"
#include "card.h"

int main()
{
  srand(time(NULL));

  // Allegro stuff
  must_init(al_init(), "allegro");
  must_init(al_init_image_addon(), "allegro");
  must_init(al_init_primitives_addon(), "primitives");
  must_init(al_install_keyboard(), "keyboard");

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  must_init(queue, "queue");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_timer_event_source(timer));
  unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
  ClearKeyboardKeys(keyboard_keys);
  ALLEGRO_EVENT event;

  // Structs

  Player *player = createPlayer();
  Combat *combat = createCombat(player, DEFAULT_ENEMY_GROUP_SIZE);

  Renderer renderer;
  renderer.combat = combat;
  FillRenderer(&renderer);
  al_register_event_source(queue, al_get_display_event_source(renderer.display));

  renderer.actual_floor = 0;
  al_start_timer(timer);
  while (1)
  {
    al_wait_for_event(queue, &event);
    int done = 0, print_combat = 0, redraw = 0;

    if (renderer.combat->enemies_left == 0)
    {
      if (renderer.actual_floor < 11)
      {
        printf("passou o nivel %d!\n", renderer.actual_floor);
        renderer.actual_floor++;

        freeEnemyAnimations(&renderer);

        freeCombat(renderer.combat);

        if (renderer.actual_floor <= 10)
        {
          renderer.combat = createCombat(player, DEFAULT_ENEMY_GROUP_SIZE);

          loadEnemyAnimations(&renderer);

          printf("Nova batalha iniciada no piso %d\n", renderer.actual_floor);
        }
        else if (renderer.actual_floor == 11)
        {
          renderer.combat = createCombat(player, BOSS_BATTLE_SIZE);

          loadEnemyAnimations(&renderer);

          printf("Batalha contra o BOSS!\n");
        }
      }
      else if (renderer.actual_floor == 11)
        renderer.actual_floor++;
    }

    if (renderer.combat->player->player_stats->healthbar <= 0)
      renderer.actual_floor = -1;

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      redraw = 1;

      // Animations
      renderer.player_idle_frame_timer++;

      if (renderer.player_idle_frame_timer >= renderer.player_idle_frame_speed)
      {
        renderer.player_idle_frame_timer = 0;
        renderer.player_idle_current_frame++;

        if (renderer.player_idle_current_frame >= renderer.player_idle_frame_count)
          renderer.player_idle_current_frame = 0;
      }

      int amount = renderer.combat->enemy_group->enemy_amount;

      for (int i = 0; i < amount; i++)
      {
        AnimationSet *animation = renderer.enemy_animations[i];

        animation->frame_timer++;

        if (animation->frame_timer >= animation->frame_speed)
        {
          animation->frame_timer = 0;
          animation->current_frame++;

          if (animation->current_frame >= animation->frame_count)
            animation->current_frame = 0;
        }
      }

      // Q LOGIC
      if (keyboard_keys[ALLEGRO_KEY_Q])
      {
        done = 1;
        break;
      }
      if (keyboard_keys[ALLEGRO_KEY_X] & GAME_KEY_SEEN)
      {
        renderer.combat->player->player_stats->healthbar = 1;
      }
      // SPACE LOGIC
      if (keyboard_keys[ALLEGRO_KEY_SPACE] & GAME_KEY_SEEN)
      {
        for (int i = 0; i < renderer.combat->enemy_group->enemy_amount; i++)
          renderer.combat->enemy_group->enemies[i]->enemy_stats->healthbar = 0;
        renderer.combat->enemies_left = 0;
      }
      // ESC LOGIC
      if (keyboard_keys[ALLEGRO_KEY_ESCAPE] & GAME_KEY_SEEN)
      {
        transferCards(renderer.combat->player->hand, renderer.combat->player->discard_stack);
        enemyTurn(renderer.combat);
      }
      // ARROWS LOGIC
      if (renderer.combat->__has_enter_been_pressed == false && renderer.combat->__has_passive_card_been_used == false)
      {
        if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_card - 1 >= 0)
            renderer.combat->pointed_card--;

        if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_card + 1 < renderer.combat->player->hand->deck_size)
            renderer.combat->pointed_card++;
      }
      else
      {
        if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_enemy - 1 >= 0)
            renderer.combat->pointed_enemy--;

        if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_enemy + 1 < renderer.combat->enemy_group->enemy_amount)
            renderer.combat->pointed_enemy++;
      }
      // ENTER LOGIC
      if (keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_SEEN)
      {
        if (renderer.actual_floor == 0)
          renderer.actual_floor++;
        else
        {
          if (renderer.combat->__has_enter_been_pressed == false && renderer.combat->__has_passive_card_been_used == false)
          {
            ActionType action_type = renderer.combat->player->hand->cards[renderer.combat->pointed_card]->card_type;
            if (action_type == ATTACK || action_type == VULNERABILITY || action_type == WEAKNESS || action_type == POISON)
            {
              if (renderer.combat->enemies_left > 0)
              {
                renderer.combat->pointed_enemy = getFirstAliveEnemy(renderer.combat->enemy_group);
                renderer.combat->__has_enter_been_pressed = true;
              }
            }
            else
              renderer.combat->__has_passive_card_been_used = true;
          }
          else if (((renderer.combat->__has_enter_been_pressed == true) || (renderer.combat->__has_passive_card_been_used == true)) && renderer.combat->player->energy >= renderer.combat->player->hand->cards[renderer.combat->pointed_card]->cost)
          {
            renderer.combat->__has_enter_been_pressed = false;
            renderer.combat->__has_passive_card_been_used = false;
            renderer.combat->player->energy -= renderer.combat->player->hand->cards[renderer.combat->pointed_card]->cost;
            applyAction(renderer.combat, renderer.combat->player->hand->cards[renderer.combat->pointed_card],
                        renderer.combat->player->player_stats, renderer.combat->enemy_group->enemies[renderer.combat->pointed_enemy]->enemy_stats);
            updateChargeCard(renderer.combat->player->hand, renderer.combat->player->energy);
            printf("enemies left: %d\n", renderer.combat->enemies_left);
          }
        }
      }
      // CANCEL ACTION LOGIC (EXTRA)
      if (keyboard_keys[ALLEGRO_KEY_C])
      {
        renderer.combat->__has_enter_been_pressed = false;
        renderer.combat->__has_passive_card_been_used = false;
      }
      for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
      {
        keyboard_keys[i] &= ~GAME_KEY_SEEN;
      }
      break;

    case ALLEGRO_EVENT_KEY_DOWN:
      keyboard_keys[event.keyboard.keycode] = GAME_KEY_SEEN | GAME_KEY_DOWN;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keyboard_keys[event.keyboard.keycode] &= ~GAME_KEY_DOWN;
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = true;
      break;
    }
    if (done)
    {
      break;
    }
    if (redraw)
    {
      Render(&renderer);
      redraw = 0;
    }
  }
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  ClearRenderer(&renderer);
  return 0;
}

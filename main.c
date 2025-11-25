#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "combat.h"

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
  Renderer renderer;
  FillRenderer(&renderer);
  al_register_event_source(queue, al_get_display_event_source(renderer.display));

  Player *player = createPlayer(PLAYER_MAX_HEALTH, PLAYER_INIT_SHIELD);

  renderer.combat = createCombat(player, DEFAULT_ENEMY_GROUP_SIZE);

  // Combat aux variables
  int actual_floor = 1;
  bool __has_enter_been_pressed = false;
  al_start_timer(timer);
  while (1)
  {
    al_wait_for_event(queue, &event);
    int done = 0, print_combat = 0, redraw = 0;

    // Combat for the new accessed floor
    if (renderer.combat == NULL && actual_floor <= 10)
      renderer.combat = createCombat(player, DEFAULT_ENEMY_GROUP_SIZE);

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      redraw = 1;
      if (keyboard_keys[ALLEGRO_KEY_Q])
      {
        done = 1;
        break;
      }
      if (__has_enter_been_pressed == false)
      {
        if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_card - 1 >= 0)
            renderer.combat->pointed_card--;

        if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_card + 1 < renderer.combat->player->hand->deck_size)
            renderer.combat->pointed_card++;
      }
      else // VAI DAR BO DPS QUANDO MORRER O INIMIGO 0
      {
        if (keyboard_keys[ALLEGRO_KEY_LEFT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_enemy - 1 >= -1)
          {
            renderer.combat->pointed_enemy--;
            printf("inimigo atual: %d\n", renderer.combat->pointed_enemy);
          }

        if (keyboard_keys[ALLEGRO_KEY_RIGHT] & GAME_KEY_SEEN)
          if (renderer.combat->pointed_enemy + 1 < renderer.combat->enemy_group->enemy_amount)
          {
            renderer.combat->pointed_enemy++;
            printf("inimigo atual: %d\n", renderer.combat->pointed_enemy);
          }
      }
      if (keyboard_keys[ALLEGRO_KEY_ENTER] & GAME_KEY_SEEN)
      {
        if (__has_enter_been_pressed == false)
          __has_enter_been_pressed = true;
        else
        {
          // execution logic here
        }
      }
      if (keyboard_keys[ALLEGRO_KEY_C])
      {
        __has_enter_been_pressed = false;
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
    if (renderer.combat->__is_player_turn == true)
    {
    }
    else
    {
      renderer.combat->__is_player_turn = true;
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
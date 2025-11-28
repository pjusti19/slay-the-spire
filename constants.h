#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define GAME_KEY_SEEN 1
#define GAME_KEY_DOWN 2

#define DISPLAY_BUFFER_WIDTH 960.0
#define DISPLAY_BUFFER_HEIGHT 540.0

#define DISPLAY_SCALE 1.0
#define DISPLAY_WIDTH DISPLAY_SCALE *DISPLAY_BUFFER_WIDTH
#define DISPLAY_HEIGHT DISPLAY_SCALE *DISPLAY_BUFFER_HEIGHT

// Player
#define PLAYER_BEGIN_X 200
#define PLAYER_BEGIN_Y 100
#define PLAYER_RADIUS 80

#define MAX_DECK_STACK 20
#define DEFAULT_HAND_STACK 5
#define PLAYER_MAX_HEALTH 100
#define PLAYER_INIT_SHIELD 0

// Deck
#define DRAW_DECK_X 30
#define DRAW_DECK_Y 400

#define DECK_WIDTH 80
#define DECK_HEIGHT 80


//Health Bar
#define HEALTH_BAR_HEIGHT 20
#define HEALTH_BAR_BACKGROUND_EXTRA 1

#define HEALTH_BAR_RX 0
#define HEALTH_BAR_RY 0

//Player Hand
#define HAND_BEGIN_X 180
#define HAND_BEGIN_Y 380

#define CARD_WIDTH 100
#define CARD_HEIGHT 130

// You might want to use this.
typedef enum
{
  MOVE_LEFT = 0,
  MOVE_RIGHT = 1,
} CursorMovementDirection;

// Energy
#define ENERGY_GAUGE_BEGIN_X 50
#define ENERGY_GAUGE_BEGIN_Y 320
#define ENERGY_GAUGE_RADIUS 30

// Enemies
#define DEFAULT_ENEMY_GROUP_SIZE 2

#define ENEMY_BEGIN_X 550
#define ENEMY_BEGIN_Y 120

#define ENEMY_WIDTH 100
#define ENEMY_HEIGHT 100

// View
#define COLOR_BLACK al_map_rgb(0,0,0)
#define COLOR_WHITE al_map_rgb(255,255,255)
#define COLOR_RED al_map_rgb(255, 0, 0)
#define COLOR_DARK_RED al_map_rgb(192, 3, 3)
#define COLOR_AQUA_BLUE al_map_rgb(105, 237, 255)

#endif
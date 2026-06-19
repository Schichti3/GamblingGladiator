#define GAME_BUILD
#include <game.h>
#include <raylib.h>

void game_init(Game_State* game_state) {
  game_state->background_color = RED;
}

void game_reload(Game_State* game_state) {
  game_state->background_color = BLUE;
}

void game_update(Game_State* game_state) {
  ClearBackground(RED);
  DrawRectangle(10, 10, 100, 100, PURPLE);
}

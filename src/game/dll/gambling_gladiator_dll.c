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

  int velocity = 5;

  if (IsKeyDown(KEY_W)) game_state->y -= velocity;
  if (IsKeyDown(KEY_S)) game_state->y += velocity;
  if (IsKeyDown(KEY_A)) game_state->x -= velocity;
  if (IsKeyDown(KEY_D)) game_state->x += velocity;

  DrawRectangle(game_state->x, game_state->y, 100, 100, PURPLE);
  DrawCircle(game_state->x, game_state->y, 100, 100, PURPLE);
}

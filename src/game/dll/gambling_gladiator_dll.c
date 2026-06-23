#define GAME_BUILD
#include <game.h>
#include <raylib.h>
#include <client.h>

void game_init(Game_State* game_state) {
  SetTargetFPS(144);
  game_state->background_color = RED;
  client_connect();
  client_start_mainloop();
}

void game_reload(Game_State* game_state) {
  game_state->background_color = BLUE;
  client_connect();
  client_start_mainloop();
}

void game_update(Game_State* game_state) {
  ClearBackground(BLACK);

  int velocity = 5;

  if (IsKeyDown(KEY_W)) game_state->y -= velocity;
  if (IsKeyDown(KEY_S)) game_state->y += velocity;
  if (IsKeyDown(KEY_A)) game_state->x -= velocity;
  if (IsKeyDown(KEY_D)) game_state->x += velocity;

  Msg msg = {.type = 1, .data_len = 6, .data = {'h', 'a', 'l', 'l', 'o', '\0'}};

  if (IsKeyPressed(KEY_M)) client_send(&msg);

  // DrawRectangle(game_state->x, game_state->y, 100, 100, PURPLE);
  DrawCircle(game_state->x, game_state->y, 30, PURPLE);
}

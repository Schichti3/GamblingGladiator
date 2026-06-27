#define GAME_BUILD
#include <game.h>
#include <client_config.h>
#include <raylib.h>
#include <client.h>
#include <string.h>
#include <player_tick.h>
#include <game_renderer.h>

static double tick_dt;
static double tick_dt_accumulator;

static Msg msgs[128];
static uint16_t msg_count;

void game_init(Game_State* game_state) {
  (void)game_state;
  SetTargetFPS(144);

  #ifdef _WIN32
    set_client_config_by_file(".\\client_config.txt");
  #else
    set_client_config_by_file("./client_config.txt");
  #endif

  tick_dt = 1.0 / get_client_config()->tick_rate;
  tick_dt_accumulator = 0.0;

  client_start_mainloop();
}

void game_deinit(void) {
  client_stop_mainloop();
}

void game_reload(Game_State* game_state) {
  (void)game_state;
  client_start_mainloop();
}


void game_update(Game_State* game_state) {
  float dt = GetFrameTime();

  if (client_fetch_msgs(msgs, &msg_count, 128)) {
  }

  tick_dt_accumulator += dt;
  while (tick_dt_accumulator >= tick_dt) {
    player_tick(tick_dt);
    tick_dt_accumulator -= tick_dt;
  }

  render_game(game_state, tick_dt_accumulator / tick_dt);

  ClearBackground(BLACK);

  int velocity = 5;

  if (IsKeyDown(KEY_W)) game_state->y -= velocity;
  if (IsKeyDown(KEY_S)) game_state->y += velocity;
  if (IsKeyDown(KEY_A)) game_state->x -= velocity;
  if (IsKeyDown(KEY_D)) game_state->x += velocity;

  const char* str = "salam alaikum";
  Msg msg = {0};
  msg.type = 1;
  strcpy(msg.data, str);
  msg.data_len = strlen(str) + 1;

  if (IsKeyPressed(KEY_M)) client_send(&msg);

  // DrawRectangle(game_state->x, game_state->y, 50, 50, PURPLE);
  DrawCircle(game_state->x, game_state->y, 30, PURPLE);
}

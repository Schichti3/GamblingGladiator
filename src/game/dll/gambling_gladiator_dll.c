#define GAME_BUILD
#include <game.h>
#include <client_config.h>
#include <raylib.h>
#include <client.h>
#include <string.h>
#include <player_tick.h>
#include <game_renderer.h>
#include <stdlib.h>
#include <stdio.h>

static double tick_dt;
static double tick_dt_accumulator;

#define MAX_MSG_COUNT 128
static Msg msgs[MAX_MSG_COUNT];
static uint16_t msg_count;

static uint16_t client_id;

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

  if (client_fetch_msgs(msgs, &msg_count, MAX_MSG_COUNT)) {
    printf("fetching msgs (msg_count = %d)\n", msg_count);
    for (int i = 0; i < msg_count; i++) {
      printf("msg.type = %d\n", msgs[i].type);
      if (msgs[i].type == 99) {
        client_id = msgs[i].client_id;
        printf("client_id = %d\n", msgs[i].client_id);
      }
      if (msgs[i].type == 2) {
        Vector3* posis = malloc(msgs[i].data_len);
        memcpy(posis, msgs[i].data, msgs[i].data_len);
        game_state->pos = posis[client_id];
        free(posis);
      }
    }
  }

  tick_dt_accumulator += dt;
  while (tick_dt_accumulator >= tick_dt) {
    player_tick(client_id, tick_dt);
    tick_dt_accumulator -= tick_dt;
  }

  render_game(game_state, tick_dt_accumulator / tick_dt);
}

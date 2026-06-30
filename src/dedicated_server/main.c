#include "server_config.h"
#include "server_tick.h"
#include <server.h>
#include <game_types.h>
#include <os_utils.h>

Player_Data player_data;

#define MAX_MSG_COUNT 1024
static Msg msgs[MAX_MSG_COUNT];
static uint16_t msg_count;

static double tick_dt;
static double tick_dt_accumulator;
static double snapshot_dt;
static double snapshot_dt_accumulator;

int main(void) {
  tick_dt = 1.0 / get_server_config()->tick_rate;
  tick_dt_accumulator = 0.0;
  snapshot_dt = 1.0 / get_server_config()->snapshot_rate;
  server_start_mainloop();
  while (1) {
    float dt = time_now_ms();
    if (server_fetch_msgs(msgs, &msg_count, MAX_MSG_COUNT)) {
      for (int i = 0; i < msg_count; i++) {
        if (msgs[i].type == 1) {
          player_data.players[msgs[i].client_id - 1].pos.x = 0;
          player_data.players[msgs[i].client_id - 1].pos.y = 0;
          player_data.players[msgs[i].client_id - 1].pos.z = 0;
          player_data.players[msgs[i].client_id - 1].vel = 5;
          player_data.count++;
        }
      }
    }

    tick_dt_accumulator += dt;
    while (tick_dt_accumulator >= tick_dt) {
      server_tick(&player_data, dt);
      tick_dt_accumulator -= tick_dt;
    }

    snapshot_dt_accumulator += dt;
    while (snapshot_dt_accumulator >= snapshot_dt) {
      server_snapshot(dt);
      snapshot_dt_accumulator -= snapshot_dt;
    }

    sleep_ms(1);
  }
  return 0;
}

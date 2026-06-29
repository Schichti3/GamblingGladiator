#include "server_tick.h"
#include <server.h>
#include <game_types.h>
#include <os_utils.h>

Player_Data player_data;

#define MAX_MSG_COUNT 1024
static Msg msgs[MAX_MSG_COUNT];
static uint16_t msg_count;

int main(void) {
  server_start_mainloop();
  while (1) {
    if (server_fetch_msgs(msgs, &msg_count, MAX_MSG_COUNT)) {

    }

    if (false) {
      server_tick(&player_data, 0.0);
    }

    sleep_ms(1);
  }
  return 0;
}

#include "client_config.h"
#define GAME_BUILD
#include <game.h>
#include <raylib.h>
#include <client.h>
#include <string.h>
#include <stdio.h>

void game_init(Game_State* game_state) {
  (void)game_state;
  SetTargetFPS(144);
  #ifdef _WIN32
    set_client_config_by_file(".\\client_config.txt");
  #else
    set_client_config_by_file("./client_config.txt");
  #endif
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
  static Msg msgs[128];
  static uint16_t msg_count;
  if (client_fetch_msgs(msgs, &msg_count, 64)) {
    //actually do something with the messages
    for (int i = 0; i < msg_count; i++) {
      printf("message received: %.*s\n", msgs[i].data_len, msgs[i].data);
    }
  }

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

#include <player_tick.h>
#include <raylib.h>
#include <client.h>
#include <strings.h>

#define W_BIT (1 << 0)
#define S_BIT (1 << 1)
#define A_BIT (1 << 2)
#define D_BIT (1 << 3)

void player_tick(float dt) {
  (void)dt;
  Player_Tick tick = {0};
  for (int key = GetKeyPressed(); key != 0; key = GetKeyPressed()) {
    switch (key) {
      case KEY_W:
        tick.key_state = W_BIT;
        break;
      case KEY_S:
        tick.key_state = S_BIT;
        break;
      case KEY_A:
        tick.key_state = A_BIT;
        break;
      case KEY_D:
        tick.key_state = D_BIT;
        break;
      default:;
    }
  }

  Msg msg = {
    .type = 1,
    .data_len = sizeof(Player_Tick)
  };
  memcpy(&msg.data, &tick, sizeof(Player_Tick));

  client_send(&msg);
}

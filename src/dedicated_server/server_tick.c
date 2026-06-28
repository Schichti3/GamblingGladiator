#include <server_tick.h>
#include <server.h>
#include <net_com.h>

void server_tick(Player_Data* player_data, float dt) {

  Msg msg = {
    .type = 2
  };

  int offset = 0;
  for (int i = 0; i < player_data->count; i++) {
    memcpy(&msg.data + offset, &player_data->players[i].pos, sizeof(Vector3));
    offset += sizeof(Vector3);
  }

  msg.data_len = player_data->count * sizeof(Vector3);

  server_send(&msg);
}

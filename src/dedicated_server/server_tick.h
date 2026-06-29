#ifndef _SERVER_TICK_H_
#define _SERVER_TICK_H_

#include <raylib.h>
#include <stdint.h>
#include <game_types.h>

typedef struct Server_Tick {
  Vector3  client_pos[64];
  uint8_t  client_count;
} Server_Tick;

void server_tick(Player_Data* player_data, float dt);
void server_snapshot(float dt);

#endif //_SERVER_TICK_H_

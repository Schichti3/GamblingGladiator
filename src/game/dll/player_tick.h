#ifndef _PLAYER_TICK_H_
#define _PLAYER_TICK_H_

#include <stdint.h>

typedef struct Player_Tick {
  uint16_t key_state; //bitmap
  uint16_t mouse_state; //bitmap
} Player_Tick;

void player_tick(float dt);

#endif //_PLAYER_TICK_H_

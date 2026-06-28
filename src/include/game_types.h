#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include <stdint.h>
#include <raylib.h>

typedef struct Player {
  Vector3 pos;
} Player;

typedef struct Player_Data {
  Player  players[128];
  uint8_t count;
} Player_Data;

#endif //_GAME_TYPES_H_

#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include <raylib.h>
#include <game_types.h>

void render_game(Game_State* game_state, double leftover_tick_dt);

#endif //_GAME_RENDERER_H_

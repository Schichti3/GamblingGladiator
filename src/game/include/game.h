#ifndef _GAME_H_
#define _GAME_H_

#include <game_types.h>
#include <stdint.h>

#ifdef GAME_BUILD
  #ifdef _WIN32
    #define GAME_API __declspec(dllexport)
  #else
    #define GAME_API __attribute__((visibility("default")))
  #endif
#else
  #define GAME_API
#endif

typedef void (*game_init_t)(Game_State* game_state);
typedef void (*game_deinit_t)(void);
typedef void (*game_reload_t)(Game_State* game_state);
typedef void (*game_update_t)(Game_State* game_state);

GAME_API void game_init(Game_State* game_state);
GAME_API void game_deinit(void);
GAME_API void game_reload(Game_State* game_state);
GAME_API void game_update(Game_State* game_state);

#endif //_GAME_H_

#ifndef _GAME_DLL_H_
#define _GAME_DLL_H_

#include <dll_loader.h>
#include <game.h>

#include <stdio.h>
#include <stdint.h>

typedef struct Game_Code {
  Lib lib;
  game_init_t init;
  game_deinit_t deinit;
  game_reload_t reload;
  game_update_t update;
  uint64_t last_write_time;
  int valid;
} Game_Code;

static int load_game_code(Game_Code* game_code) {
  uint64_t write_time = get_file_time(GAME_LIB_PATH);
  if (write_time == 0) return 0;

  if (write_time == game_code->last_write_time && game_code->valid) {
    return 0;
  }

  sleep_ms(100);

  char temp_path[512];
  static uint64_t load_count = 0;
  int written = snprintf(
      temp_path,
      sizeof(temp_path),
      TEMP_LIB_PATH_FORMAT,
      (unsigned long long)write_time,
      (unsigned long long)(++load_count));
  if (written < 0 || (size_t)written >= sizeof(temp_path)) {
    return 0;
  }

  if (!copy_lib(GAME_LIB_PATH, temp_path)) {
    return 0;
  }

  if (game_code->valid) {
    game_code->deinit();
  }

  Lib new_lib = open_lib(temp_path);
  if (!new_lib) return 0;

  #if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
  #endif

  game_init_t init = (game_init_t)load_symbol(new_lib, "game_init");
  game_deinit_t deinit = (game_deinit_t)load_symbol(new_lib, "game_deinit");
  game_reload_t reload = (game_reload_t)load_symbol(new_lib, "game_reload");
  game_update_t update = (game_update_t)load_symbol(new_lib, "game_update");

  #if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
  #endif

  if (!init || !deinit || !reload || !update) {
    close_lib(new_lib);
    return 0;
  }

  Lib old_lib = game_code->lib;

  if (old_lib) {
    close_lib(old_lib);
  }

  game_code->lib = new_lib;
  game_code->init = init;
  game_code->deinit = deinit;
  game_code->reload = reload;
  game_code->update = update;
  game_code->last_write_time = write_time;
  game_code->valid = 1;

  return 1;
}

#endif //_GAME_DLL_H_

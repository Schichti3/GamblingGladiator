#ifndef _DLL_LOADER_H_
#define _DLL_LOADER_H_

#include <stdint.h>

typedef void* Lib;

#ifdef _WIN32
  #define GAME_LIB_PATH "libgambling-gladiator-dll.dll"
  #define TEMP_LIB_PATH_FORMAT "libgambling-gladiator-dll_temp_%llu_%llu.dll"
#endif

#ifdef __unix__
  #ifdef __APPLE__
    #define GAME_LIB_PATH "./libgambling-gladiator-dll.dylib"
    #define TEMP_LIB_PATH_FORMAT "./libgambling-gladiator-dll_temp_%llu_%llu.dylib"
  #else
    #define GAME_LIB_PATH "./libgambling-gladiator-dll.so"
    #define TEMP_LIB_PATH_FORMAT "./libgambling-gladiator-dll_temp_%llu_%llu.so"
  #endif
#endif

uint64_t get_file_time(const char* path);
int      copy_lib(const char* src, const char* dst);
Lib      open_lib(const char* path);
void*    load_symbol(Lib lib, const char* name);
void     close_lib(Lib lib);
void     sleep(unsigned int ms);

#endif //_DLL_LOADER_H_

#ifndef _DLL_LOADER_H_
#define _DLL_LOADER_H_

#include <stdint.h>
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>

  #define GAME_LIB_PATH "gambling-gladiator-dll.dll"
  #define TEMP_LIB_PATH_FORMAT "gambling-gladiator-dll_temp_%llu_%llu.dll"

  typedef HMODULE Lib;

  static uint64_t get_file_time(const char* path) {
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data)) return 0;
    ULARGE_INTEGER time;
    time.LowPart = data.ftLastWriteTime.dwLowDataTime;
    time.HighPart = data.ftLastWriteTime.dwHighDateTime;
    return time.QuadPart;
  }

  static int copy_lib(const char* src, const char* dst) {
    return CopyFileA(src, dst, FALSE);
  }

  static Lib open_lib(const char* path) {
    return LoadLibraryA(path);
  }

  static void* load_symbol(Lib lib, const char* name) {
    return (void*)GetProcAddress(lib, name);
  }

  static void close_lib(Lib lib) {
    if (lib) FreeLibrary(lib);
  }
#endif

#ifdef __unix__
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dlfcn.h>
  #include <stdio.h>

  #ifdef __APPLE__
    #define GAME_LIB_PATH "./libgambling-gladiator-dll.dylib"
    #define TEMP_LIB_PATH_FORMAT "./libgambling-gladiator-dll_temp_%llu_%llu.dylib"
  #else
    #define GAME_LIB_PATH "./libgambling-gladiator-dll.so"
    #define TEMP_LIB_PATH_FORMAT "./libgambling-gladiator-dll_temp_%llu_%llu.so"
  #endif

  typedef void* Lib;

  static uint64_t get_file_time(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (uint64_t)st.st_mtime;
  }

  static int copy_lib(const char* src, const char* dst) {
    FILE* in = fopen(src, "rb");
    if (!in) return 0;

    unlink(dst);
    FILE* out = fopen(dst, "wb");
    if (!out) {
      fclose(in);
      return 0;
    }

    char buff[4096];
    size_t n;
    while ((n = fread(buff, 1, sizeof(buff), in)) > 0) {
      fwrite(buff, 1, n, out);
    }

    fclose(out);
    fclose(in);
    return 1;
  }

  static Lib open_lib(const char* path) {
    return dlopen(path, RTLD_NOW | RTLD_LOCAL);
  }

  static void* load_symbol(Lib lib, const char* name) {
    return dlsym(lib, name);
  }

  static void close_lib(Lib lib) {
    if (lib) dlclose(lib);
  }
#endif

#endif //_DLL_LOADER_H_

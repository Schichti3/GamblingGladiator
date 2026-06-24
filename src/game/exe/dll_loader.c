#include <dll_loader.h>

#ifdef _WIN32
  #include <windows.h>

  uint64_t get_file_time(const char* path) {
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data)) return 0;
    ULARGE_INTEGER time;
    time.LowPart = data.ftLastWriteTime.dwLowDateTime;
    time.HighPart = data.ftLastWriteTime.dwHighDateTime;
    return time.QuadPart;
  }

  int copy_lib(const char* src, const char* dst) {
    return CopyFileA(src, dst, FALSE);
  }

  Lib open_lib(const char* path) {
    return (Lib)LoadLibraryA(path);
  }

  void* load_symbol(Lib lib, const char* name) {
    #if defined(__GNUC__) || defined(__clang__)
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wpedantic"
    #endif

    return (void*)GetProcAddress(lib, name);

    #if defined(__GNUC__) || defined(__clang__)
      #pragma GCC diagnostic pop
    #endif
  }

  void close_lib(Lib lib) {
    if (lib) FreeLibrary(lib);
  }

  void sleep(unsigned int ms) {
    Sleep(ms);
  }

#endif

#ifdef __unix__
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dlfcn.h>
  #include <stdio.h>

  uint64_t get_file_time(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (uint64_t)st.st_mtime;
  }

  int copy_lib(const char* src, const char* dst) {
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

  Lib open_lib(const char* path) {
    return dlopen(path, RTLD_NOW | RTLD_LOCAL);
  }

  void* load_symbol(Lib lib, const char* name) {
    return dlsym(lib, name);
  }

  void close_lib(Lib lib) {
    if (lib) dlclose(lib);
  }

  void sleep(unsigned int ms) {
    usleep(ms * 1000);
  }
#endif

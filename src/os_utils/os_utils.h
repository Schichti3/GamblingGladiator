#ifndef _OS_UTILS_H_
#define _OS_UTILS_H_

#include <stdint.h>

typedef void* Lib;

uint64_t get_file_time(const char* path);
int      copy_lib(const char* src, const char* dst);
Lib      open_lib(const char* path);
void*    load_symbol(Lib lib, const char* name);
void     close_lib(Lib lib);

void     sleep_ms(unsigned int ms);
double   time_now_ms(void);

#endif //_OS_UTILS_H_

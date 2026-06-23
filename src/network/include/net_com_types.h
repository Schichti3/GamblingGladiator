#ifndef _NET_COM_TYPES_H_
#define _NET_COM_TYPES_H_

#include <stdint.h>

typedef struct Msg {
uint64_t type;
uint64_t data_len;
uint8_t  data[128];
} Msg;

#endif //_NET_COM_TYPES_H_

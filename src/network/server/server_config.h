#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include <stdint.h>

typedef struct {
  uint16_t     port;
  const char*  ip;
} Server_Config;

Server_Config* get_server_config(void);

#endif //_SERVER_CONFIG_H_

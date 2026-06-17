#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include <stdint.h>

typedef struct {
  uint16_t     port;
  const char*  ip;
  uint8_t      max_clients;
  uint8_t      channels;
} Server_Config;

Server_Config* get_server_config(void);

#endif //_SERVER_CONFIG_H_

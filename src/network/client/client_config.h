#ifndef _CLIENT_CONFIG_
#define _CLIENT_CONFIG_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t     port;
  const char*  ip;
  uint8_t      max_channels;
} Client_Config;

Client_Config* get_client_config(void);
bool set_client_config_by_file(const char* path);

#endif //_CLIENT_CONFIG_

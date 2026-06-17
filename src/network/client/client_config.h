#ifndef _CLIENT_CONFIG_
#define _CLIENT_CONFIG_

#include <stdint.h>

typedef struct {
  uint16_t     port;
  const char*  ip;
} Client_Config;

Client_Config* get_client_config(void);

#endif //_CLIENT_CONFIG_

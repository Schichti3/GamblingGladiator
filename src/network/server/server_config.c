#include "server_config.h"

static Server_Config server_config = {
  6969,
  "127.0.0.1",
};

Server_Config* get_server_config(void) {
  return &server_config;
}

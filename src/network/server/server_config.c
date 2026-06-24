#include "server_config.h"

static Server_Config server_config = {
  .port = 6969,
  .ip = "0.0.0.0",
  .max_clients = 32,
  .channels = 2
};

Server_Config* get_server_config(void) {
  return &server_config;
}

#include "client_config.h"

static Client_Config client_config = {
  .port = 6969,
  .ip = "127.0.0.1",
  .max_channels = 2
};

Client_Config* get_client_config(void) {
  return &client_config;
}

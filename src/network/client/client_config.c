#include "client_config.h"

static Client_Config client_config = {
  6969,
  "127.0.0.1",
};

Client_Config* get_client_config(void) {
  return &client_config;
}

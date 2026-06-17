#include "client.h"
#include "client_config.h"

Error_Code client_connect(void) {
  Client_Config* conf = get_client_config();
  (void)conf;
  return ALL_GOOD;
}



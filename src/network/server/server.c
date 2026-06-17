#include "server.h"
#include "server_config.h"

Error_Code server_run(void) {
  Server_Config* conf = get_server_config();
  (void)conf;
  return ALL_GOOD;
}

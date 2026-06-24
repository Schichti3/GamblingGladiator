#include "client_config.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Client_Config client_config = {
  .port = 6969,
  .ip = "127.0.0.1",
  .max_channels = 2
};

Client_Config* get_client_config(void) {
  return &client_config;
}

bool set_client_config_by_file(const char* path)
{
  FILE* file = fopen(path, "r");
  if (!file) {
    printf("client_config.txt file doesn't exist!\n");
    return false;
  }

  printf("loading client_config.txt\n");

  char ip[256];
  char port[256];

  fgets(ip, sizeof(ip), file);
  fgets(port, sizeof(port), file);

  ip[strcspn(ip, "\r\n")] = '\0';
  port[strcspn(ip, "\r\n")] = '\0';

  client_config.ip = ip;
  client_config.port = atoi(port);

  printf("ip = %s\n", ip);
  printf("port = %d\n", atoi(port));

  return true;
}

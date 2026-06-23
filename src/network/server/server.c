#include "server.h"
#include <error_codes.h>
#include <net_com_types.h>
#include "server_config.h"

#include <stdio.h>
#include <string.h>

Error_Code server_run(void) {
  Server_Config* conf = get_server_config();

  if (enet_initialize() != 0) {
    return INITIALIZING_SERVER_FAILED;
  }

  ENetAddress address;
  enet_address_set_host_ip(&address, conf->ip);
  address.port = conf->port;

  ENetHost* server = enet_host_create(
    &address,
    conf->max_clients,
    conf->channels,
    0,
    0
  );

  if (server == NULL) {
    return SERVER_CREATION_FAILED;
  }

  ENetEvent event;

  while (1) {
    while (enet_host_service(server, &event, 1000)) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          log_client_connected(&event);
          break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
          printf("Packet received\n");
          Msg msg = {0};
          memcpy(&msg, event.packet->data, event.packet->dataLength);
          printf("type     from msg: %d\n", (int)msg.type );
          printf("data_len from msg: %d\n", (int)msg.data_len );
          printf("data     from msg: %s\n", msg.data );
          break;
        case ENET_EVENT_TYPE_DISCONNECT: {
          log_client_disconnected(&event);
          break;
        }
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }

  enet_host_destroy(server);

  return ALL_GOOD;
}

void log_client_connected(ENetEvent* event) {
  char client_ip[32] = {0};
  if (enet_address_get_host_ip(&event->peer->address, client_ip, 32) == 0) {
    printf("Client connected from %s:%u\n",
           client_ip,
           event->peer->address.port
    );
  }
}

void log_client_disconnected(ENetEvent* event) {
  char client_ip[32] = {0};
  if (enet_address_get_host_ip(&event->peer->address, client_ip, 32) == 0) {
    printf("Client from %s:%u disconnected\n",
           client_ip,
           event->peer->address.port
    );
  }
}

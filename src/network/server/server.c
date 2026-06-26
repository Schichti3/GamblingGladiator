#include "server.h"
#include <error_codes.h>
#include <net_com.h>
#include "server_config.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool server_running;

static void log_client_connected(ENetEvent* event) {
  char client_ip[32] = {0};
  if (enet_address_get_host_ip(&event->peer->address, client_ip, 32) == 0) {
    printf("Client connected from %s:%u\n",
           client_ip,
           event->peer->address.port
    );
  }
}

static void log_client_disconnected(ENetEvent* event) {
  char client_ip[32] = {0};
  if (enet_address_get_host_ip(&event->peer->address, client_ip, 32) == 0) {
    printf("Client from %s:%u disconnected\n",
           client_ip,
           event->peer->address.port
    );
  }
}

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

  server_running = true;

  while (server_running) {
    while (enet_host_service(server, &event, 1000)) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
          log_client_connected(&event);
          const char* hello = "Greetings, client!";
          Msg msg;
          msg.type = 187;
          strcpy(msg.data, hello);
          msg.data_len = strlen(hello) + 1;
          ENetPacket* packet = enet_packet_create(&msg, sizeof(Msg), ENET_PACKET_FLAG_RELIABLE);
          enet_peer_send(event.peer, 0, packet);
          enet_host_flush(server);
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


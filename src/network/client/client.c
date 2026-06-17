#include "client.h"
#include "client_config.h"
#include "error_codes.h"

#include <enet/enet.h>
#include <stdio.h>
#include <string.h>

Error_Code client_connect(void) {
  Client_Config* conf = get_client_config();

  if (enet_initialize() != 0) {
    return INITIALIZING_CLIENT_FAILED;
  }

  atexit(enet_deinitialize);

  ENetHost* client = enet_host_create(
    NULL,
    1,
    conf->max_channels,
    0,
    0
  );

  if (client == NULL) {
    return CLIENT_CREATION_FAILED;
  }

  ENetAddress address;

  enet_address_set_host(&address, conf->ip);
  address.port = conf->port;

  ENetPeer* peer = enet_host_connect(
    client,
    &address,
    conf->max_channels,
    0
  );

  if (peer == NULL) {
    enet_host_destroy(client);
    printf("Creating connection failed\n");
    return CLIENT_CREATING_CONNECTION_FAILED;
  }

  ENetEvent event;

  // Wait up to 5 seconds for connection.
  if (enet_host_service(client, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    printf("Connected to server\n");
  } else {
    printf("Connection failed\n");
    enet_peer_reset(peer);
    enet_host_destroy(client);
    return CLIENT_CREATING_CONNECTION_FAILED;
  }

  enet_host_flush(client);

  return ALL_GOOD;
}



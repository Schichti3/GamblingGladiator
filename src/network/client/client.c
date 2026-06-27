#define NET_COM_FUNCTIONS
#include <net_com.h>
#include "client.h"
#include "client_config.h"
#include "error_codes.h"

#include <pthread.h>
#include <thread.h>

#include <enet/enet.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ring_buf.h>

Client_Config* client_config;

static ENetHost* client;
static ENetPeer* server;

static atomic_bool mainloop_running;
static Thread_Handle mainloop_thread_handle;

STANDARD_RING_BUF_IMPL(Msg_Buf, Msg, 128)
static Msg_Buf msg_buf;
static pthread_mutex_t msg_buf_lock;


static Error_Code client_create(void) {
  client_config = get_client_config();

  Msg_Buf_init(&msg_buf);
  pthread_mutex_init(&msg_buf_lock, NULL);

  if (enet_initialize() != 0) {
    return INITIALIZING_CLIENT_FAILED;
  }

  atexit(enet_deinitialize);

  client = enet_host_create(
    NULL,
    1,
    client_config->max_channels,
    0,
    0
  );

  if (!client) {
    return CLIENT_CREATION_FAILED;
  }

  ENetAddress address;

  enet_address_set_host(&address, client_config->ip);
  address.port = client_config->port;

  server = enet_host_connect(
    client,
    &address,
    client_config->max_channels,
    0
  );

  if (!server) {
    enet_host_destroy(client);
    printf("Creating connection failed\n");
    return CLIENT_CREATING_CONNECTION_FAILED;
  }

  return ALL_GOOD;
}

Error_Code client_send(Msg* msg) {
  if (!client) return CLIENT_SEND_FAILED_NO_CLIENT;
  if (!server) return CLIENT_SEND_FAILED_NO_SERVER;
  ENetPacket* packet = enet_packet_create(msg, sizeof(Msg), ENET_PACKET_FLAG_RELIABLE);
  int res = enet_peer_send(server, 0, packet);
  enet_host_flush(client);
  if (res == 0) {
    return ALL_GOOD;
  } else {
    return CLIENT_SEND_FAILED;
  }
}

static void* mainloop(void* args) {
  (void)args;
  ENetEvent event;
  Msg msg;
  while (mainloop_running) {
    if (enet_host_service(client, &event, 1000) > 0) {
      switch(event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          //TODO add connection established info in game? maybe some kind of popup / notification
          printf("Connected to server\n");
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          packet_to_msg(&msg, event.packet);
          pthread_mutex_lock(&msg_buf_lock);
          Msg_Buf_push(&msg_buf, msg);
          pthread_mutex_unlock(&msg_buf_lock);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          break;
        case ENET_EVENT_TYPE_NONE:
          printf("test\n");
          break;
      }
    }
  }
  enet_peer_disconnect(server, 0);
  enet_peer_reset(server);
  enet_host_destroy(client);
  return 0;
}

Error_Code client_start_mainloop(void) {
  if (atomic_load(&mainloop_running) == true) {
    return CLIENT_MAINLOOP_ALREADY_RUNNING;
  }
  Error_Code client_creation = client_create();
  if (client_creation != ALL_GOOD) {
    return client_creation;
  }
  atomic_store(&mainloop_running, true);
  Thread_Handle handle;
  Thread_Error res = thread_run(mainloop, 0, &handle);
  if (res != THREAD_SUCCESS) {
    mainloop_thread_handle = 0;
    atomic_store(&mainloop_running, false);
    return CLIENT_MAINLOOP_START_FAILED;
  }

  mainloop_thread_handle = handle;
  return ALL_GOOD;
}

Error_Code client_stop_mainloop(void) {
  atomic_store(&mainloop_running, false);
  if (thread_join(mainloop_thread_handle) != THREAD_SUCCESS) {
    return CLIENT_MAINLOOP_STOP_FAILED;
  }

  return ALL_GOOD;
}

bool client_fetch_msgs(Msg* msgs, uint16_t* msg_count, uint16_t max_msg_count) {
  *msg_count = 0;
  Msg msg;
  pthread_mutex_lock(&msg_buf_lock);
  while (Msg_Buf_pop(&msg_buf, &msg)) {
    *msgs = msg;
    ++msgs;
    if (++(*msg_count) == max_msg_count) break;
  }
  pthread_mutex_unlock(&msg_buf_lock);

  return (*msg_count) > 0;
}

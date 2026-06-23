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

static ENetHost* client;
static ENetPeer* server;

static atomic_bool mainloop_running;
static Thread_Handle mainloop_thread_handle;

#define MAX_MSG_BUF_SIZE 64

typedef struct Msg_Buf {
  Msg buf[MAX_MSG_BUF_SIZE];
  uint16_t msg_count;
} Msg_Buf;

static Msg_Buf msg_buf = {0};
pthread_mutex_t msg_buf_lock = PTHREAD_MUTEX_INITIALIZER;

Error_Code client_connect(void) {
  Client_Config* conf = get_client_config();

  if (enet_initialize() != 0) {
    return INITIALIZING_CLIENT_FAILED;
  }

  atexit(enet_deinitialize);

  client = enet_host_create(
    NULL,
    1,
    conf->max_channels,
    0,
    0
  );

  if (!client) {
    return CLIENT_CREATION_FAILED;
  }

  ENetAddress address;

  enet_address_set_host(&address, conf->ip);
  address.port = conf->port;

  server = enet_host_connect(
    client,
    &address,
    conf->max_channels,
    0
  );

  if (!server) {
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
    enet_peer_reset(server);
    enet_host_destroy(client);
    server = NULL;
    client = NULL;
    return CLIENT_CREATING_CONNECTION_FAILED;
  }

  enet_host_flush(client);

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
  while (mainloop_running) {
    if (enet_host_service(client, &event, 5000) > 0) {
      switch(event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          //TODO add connection established info in game? maybe some kind of popup / notification
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          pthread_mutex_lock(&msg_buf_lock);
          //do stuff / TODO add msg to bsg_buf
          pthread_mutex_unlock(&msg_buf_lock);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }
  return 0;
}

Error_Code client_start_mainloop(void) {
  if (atomic_load(&mainloop_running) == true) {
    return CLIENT_MAINLOOP_ALREADY_RUNNING;
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

bool client_fetch_msgs(Msg* msgs, uint16_t* msg_count) {
  pthread_mutex_lock(&msg_buf_lock);
  if (msg_buf.msg_count == 0) {
    pthread_mutex_unlock(&msg_buf_lock);
    return false;
  }

  msgs = malloc(sizeof(Msg) * msg_buf.msg_count);
  *msg_count = msg_buf.msg_count;

  for (int i = 0; i < msg_buf.msg_count; ++i) {
    msgs[i] = msg_buf.buf[i];
  }

  pthread_mutex_unlock(&msg_buf_lock);
  return true;
}

void client_free_msg(Msg* msg) {
  (void)msg; //TODO maybe comment in again, depending on how I structure the code
  // free(msg->data);
  // free(msg);
}

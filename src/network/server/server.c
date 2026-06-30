#define NET_COM_FUNCTIONS
#include <net_com.h>
#include <server.h>
#include <error_codes.h>
#include <server_config.h>
#include <enet/enet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <thread.h>
#include <ring_buf.h>
#include <stdlib.h>

Server_Config* server_config;

static ENetHost* server;
static ENetPeer** clients;
static uint16_t client_count;

static atomic_bool mainloop_running;
static Thread_Handle mainloop_thread_handle;

STANDARD_RING_BUF_IMPL(Msg_Buf, Msg, 1024)
static Msg_Buf msg_buf;
static pthread_mutex_t msg_buf_lock;

static Error_Code server_create(void) {
  server_config = get_server_config();

  if (!clients) {
    clients = calloc(server_config->max_clients, sizeof(ENetPeer*));
  }

  Msg_Buf_init(&msg_buf);
  pthread_mutex_init(&msg_buf_lock, NULL);

  if (enet_initialize() != 0) {
    return INITIALIZING_SERVER_FAILED;
  }

  atexit(enet_deinitialize);

  ENetAddress address;
  enet_address_set_host_ip(&address, server_config->ip);
  address.port = server_config->port;

  server = enet_host_create(
    &address,
    server_config->max_clients,
    server_config->channels,
    0,
    0
  );

  if (!server) {
    enet_host_destroy(server);
    return SERVER_CREATION_FAILED;
  }

  return ALL_GOOD;
}

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

Error_Code server_send(Msg* msg) {
  if (!server) return SERVER_SEND_FAILED_NO_SERVER;
  ENetPeer* client = clients[msg->client_id]; //could cause access violation if client_id is not in bounds of clients
  if (!client) return SERVER_SEND_FAILED_NO_VALID_CLIENT_ID;
  ENetPacket* packet = enet_packet_create(msg, sizeof(Msg), ENET_PACKET_FLAG_RELIABLE);
  int res = enet_peer_send(client, 0, packet);
  enet_host_flush(server);
  if (res == 0) {
    return ALL_GOOD;
  } else {
    return CLIENT_SEND_FAILED;
  }
}

Error_Code server_broadcast(Msg* msg) {
  if (!server) return SERVER_BROADCAST_FAILED_NO_SERVER;
  ENetPacket* packet = enet_packet_create(msg, sizeof(Msg), ENET_PACKET_FLAG_RELIABLE);
  enet_host_broadcast(server, 0, packet);
  enet_host_flush(server);
  return ALL_GOOD;
}

static void* mainloop(void* args) {
  (void)args;
  ENetEvent event;
  Msg msg;
  while (mainloop_running) {
    if (enet_host_service(server, &event, 1000) > 0) {
      switch(event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          log_client_connected(&event);
          printf("Connected to server\n");
          msg.client_id = client_count++;
          clients[msg.client_id] = event.peer;
          msg.type = 99; //TODO should be changed
          pthread_mutex_lock(&msg_buf_lock);
          Msg_Buf_push(&msg_buf, msg);
          pthread_mutex_unlock(&msg_buf_lock);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          packet_to_msg(&msg, event.packet);
          pthread_mutex_lock(&msg_buf_lock);
          Msg_Buf_push(&msg_buf, msg);
          pthread_mutex_unlock(&msg_buf_lock);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          log_client_disconnected(&event);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }

  enet_host_destroy(server);
  return 0;
}

Error_Code server_start_mainloop(void) {
  if (atomic_load(&mainloop_running) == true) {
    return SERVER_MAINLOOP_ALREADY_RUNNING;
  }
  Error_Code server_creation = server_create();
  if (server_creation != ALL_GOOD) {
    return server_creation;
  }
  atomic_store(&mainloop_running, true);
  Thread_Handle handle;
  Thread_Error res = thread_run(mainloop, 0, &handle);
  if (res != THREAD_SUCCESS) {
    mainloop_thread_handle = 0;
    atomic_store(&mainloop_running, false);
    return SERVER_MAINLOOP_START_FAILED;
  }

  mainloop_thread_handle = handle;
  return ALL_GOOD;
}

Error_Code server_stop_mainloop(void) {
  atomic_store(&mainloop_running, false);
  if (thread_join(mainloop_thread_handle) != THREAD_SUCCESS) {
    return CLIENT_MAINLOOP_STOP_FAILED;
  }

  return ALL_GOOD;
}

bool server_fetch_msgs(Msg* msgs, uint16_t* msg_count, uint16_t max_msg_count) {
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

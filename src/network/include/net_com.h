#ifndef _NET_COM_H_
#define _NET_COM_H_

#include <stdint.h>
#include <string.h>

typedef struct Msg {
  uint16_t type;
  uint16_t data_len;
  char     data[128];
} Msg;

#ifdef NET_COM_FUNCTIONS
  #include <enet/enet.h>
  static inline void packet_to_msg(Msg* msg, const ENetPacket* packet) {
    uint16_t offset = 0;
    memcpy(&msg->type, packet->data, sizeof(msg->type));
    offset += sizeof(msg->type);
    memcpy(&msg->data_len, packet->data + offset, sizeof(msg->data_len));
    offset += sizeof(msg->data_len);
    memcpy(&msg->data,     packet->data + offset, msg->data_len);
  }
#endif

#endif //_NET_COM_H_

#ifndef _SERVER_H_
#define _SERVER_H_
#include <error_codes.h>
#include <enet/enet.h>

Error_Code server_run(void);

void log_client_connected(ENetEvent* event);
void log_client_disconnected(ENetEvent* event);

#endif //_SERVER_H_

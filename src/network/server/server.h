#ifndef _SERVER_H_
#define _SERVER_H_
#include <error_codes.h>
#include <net_com.h>
#include <stdint.h>
#include <stdbool.h>

Error_Code server_send(Msg* msg);
Error_Code server_broadcast(Msg* msg);
Error_Code server_start_mainloop(void);
Error_Code server_stop_mainloop(void);
bool       server_fetch_msgs(Msg* msgs,
                             uint16_t* msg_count,
                             uint16_t max_msg_count);
#endif //_SERVER_H_

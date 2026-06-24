#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <error_codes.h>
#include <net_com.h>
#include <stdint.h>
#include <stdbool.h>

Error_Code client_send(Msg* msg);
Error_Code client_start_mainloop(void);
Error_Code client_stop_mainloop(void);
bool client_fetch_msgs(Msg* msgs, uint16_t* msg_count, uint16_t max_msg_count);

#endif //_CLIENT_H_

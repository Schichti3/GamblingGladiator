#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <error_codes.h>
#include <net_com_types.h>
#include <stdint.h>
#include <stdbool.h>

Error_Code client_connect(void);
Error_Code client_send(Msg* msg);
Error_Code client_start_mainloop(void);
Error_Code client_stop_mainloop(void);
bool client_fetch_msgs(Msg* msgs, uint16_t* msg_count);
void client_free_msg(Msg* msg);

#endif //_CLIENT_H_

#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <error_codes.h>
#include <stdint.h>

Error_Code client_connect(void);
Error_Code client_send(void* data, uint32_t data_len);

#endif //_CLIENT_H_

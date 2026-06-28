#include <server.h>
#include <stdio.h>

#define MAX_MSG_COUNT 1024
static Msg msgs[MAX_MSG_COUNT];
static uint16_t msg_count;

int main(void) {
  server_start_mainloop();
  while (1) {
    if (server_fetch_msgs(msgs, &msg_count, MAX_MSG_COUNT)) {

    }
  }
  return 0;
}

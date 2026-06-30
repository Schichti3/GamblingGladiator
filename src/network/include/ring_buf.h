#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

//TODO safety checks, nothing is checked yet

#define STANDARD_RING_BUF_IMPL(name, type, size)              \
  enum { name##_real_size = (size) + 1};                      \
  typedef struct name {                                       \
    type     buf[name##_real_size];                           \
    uint16_t head_idx;                                        \
    uint16_t tail_idx;                                        \
  } name;                                                     \
                                                              \
  static inline void name##_init(name *b) {                   \
    b->head_idx = 0;                                          \
    b->tail_idx = 0;                                          \
  }                                                           \
                                                              \
  static inline void name##_push(name *b, type val) {         \
    uint16_t next_tail_idx = b->tail_idx + 1;                 \
    if (next_tail_idx == name##_real_size) next_tail_idx = 0; \
    if (next_tail_idx == b->head_idx) assert("ring_buf tail would reach head, ring buf is full"); \
    b->buf[b->tail_idx] = val;                                \
    b->tail_idx = next_tail_idx;                              \
  }                                                           \
                                                              \
  static inline bool name##_pop(name *b, type *dst) {         \
    if (b->head_idx == b->tail_idx) return false;             \
    uint16_t next_head_idx = b->head_idx + 1;                 \
    if (next_head_idx == name##_real_size) next_head_idx = 0; \
    *dst = b->buf[b->head_idx];                               \
    b->head_idx = next_head_idx;                              \
    return true;                                              \
  }



#endif //_RING_BUF_H_

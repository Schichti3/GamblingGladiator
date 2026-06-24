#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdint.h>
#include <assert.h>

//TODO safety checks, nothing is checked yet

#define STANDARD_RING_BUF_IMPL(name, type, size)        \
  typedef struct name {                                 \
    type     buf[size];                                 \
    uint16_t head_idx;                                  \
    uint16_t tail_idx;                                  \
  } name;                                               \
                                                        \
  static inline void name##_push(name *b, type val) {   \
    if (b->tail_idx + 1 == b->head_idx) assert(!"ring_buf tail reached head, ring buf overflow"); \
    b->buf[b->tail_idx] = val;                          \
    ++b->tail_idx;                                      \
    if ( b->tail_idx == size - 1 ) b->tail_idx = 0;     \
  }                                                     \
                                                        \
  static inline bool name##_pop(name *b, type *dst) {   \
    if (b->head_idx == b->tail_idx) return false;       \
    *dst = b->buf[b->head_idx];                         \
    ++b->head_idx;                                      \
    if ( b->head_idx == size - 1 ) b->head_idx = 0;     \
    return true;                                        \
  }



#endif //_RING_BUF_H_

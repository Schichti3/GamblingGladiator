#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdint.h>

typedef enum Thread_Error {
  THREAD_SUCCESS,
  THREAD_CREATION_FAILED,
  THREAD_JOINING_FAILED
} Thread_Error;

typedef uint64_t Thread_Handle;

typedef void*(*Thread_Func)(void*);

Thread_Error thread_run(Thread_Func func, void* args, Thread_Handle* handle);
Thread_Error thread_join(Thread_Handle handle);

#endif //_THREAD_H_

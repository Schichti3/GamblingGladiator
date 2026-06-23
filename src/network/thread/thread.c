#include <thread.h>

Thread_Error thread_run(Thread_Func func, void* args, Thread_Handle* handle) {
  pthread_t thread;
  int result = pthread_create(
    &thread,
    NULL,
    func,
    args
  );
  if (result != 0) {
    *handle = 0;
    return THREAD_CREATION_FAILED;
  }
  *handle = thread;
  return THREAD_SUCCESS;
}

Thread_Error thread_join(Thread_Handle handle) {
  int res = pthread_join(handle, NULL);
  if (res != 0) {
    return THREAD_JOINING_FAILED;
  }
  return THREAD_SUCCESS;
}


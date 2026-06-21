#include <thread.h>

#ifdef _WIN32
  #include <windows.h>
  Thread_Error thread_run(Thread_Func func, void* args, Thread_Handle* handle) {
    DWORD thread_id;
    HANDLE thread = CreateThread(
      NULL,
      0,
      func,
      args,
      0,
      &thread_id
    );

    if (!thread) {
      *handle = 0;
      return THREAD_CREATION_FAILED;
    }

    *handle = thread;
    return THREAD_SUCCESS;
  }
  Thread_Error thread_join(Thread_Handle handle) {
    DWORD res = WaitForSingleObject((HANDLE)handle, INFINITE);

    if (res != WAIT_OBJECT_0) {
      CloseHandle((HANDLE)handle);
      return THREAD_JOINING_FAILED;
    }

    CloseHandle((HANDLE)handle);
    return THREAD_SUCCESS;
  }
#endif

#ifdef __unix__
  #include <pthread.h>
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
#endif

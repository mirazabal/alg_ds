#ifndef TASK_THREAD_POOL_MIR_H
#define  TASK_THREAD_POOL_MIR_H 

typedef struct{
  void* args;
  void (*func)(void* args);
} task_t;

#endif


#ifndef TASK_MANAGER_WORKING_STEALING_H
#define TASK_MANAGER_WORKING_STEALING_H 

#include "task.h"
#include "notification_queue.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

typedef struct{

  pthread_t* t_arr;
  size_t len_thr;
  
  atomic_uint_fast64_t index;
//  _Atomic uint64_t index;

  not_q_t* q_arr;

} task_manager_t;

void init_task_manager(task_manager_t* man, uint32_t num_threads);

void free_task_manager(task_manager_t* man, void (*clean)(void* args) );

void async_task_manager(task_manager_t* man, task_t t);

#endif


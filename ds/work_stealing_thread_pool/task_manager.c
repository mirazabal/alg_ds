#include "task_manager.h"

#include <assert.h> 
#include <stdlib.h>

typedef struct{
  task_manager_t* man;
  int idx;
} task_thread_args_t;

static
void* worker_thread(void* arg)
{
  assert(arg != NULL);

  task_thread_args_t* args = (task_thread_args_t*)arg; 

  int const idx = args->idx;
  task_manager_t* man = args->man;

  while(true){

    ret_try_t ret = {.success = false}; 

    for(uint32_t i = idx; i < man->len_thr + idx; ++i){
      ret = try_pop_not_q(&man->q_arr[i%man->len_thr]);
      if(ret.success == true){
        //printf("Try pop succeed try pop val = %d idx = %ld \n", *(int*)ret.t.args, i%man->len_thr);
        break;
      } 
    }

    if(ret.success == false && pop_not_q( &man->q_arr[idx], &ret.t) == false)
      break;

    //printf("After pop_not_q val = %d idx = %d \n", *(int*)ret.t.args, idx );

    ret.t.func(ret.t.args); 
  }

  free(args);
  return NULL;
}

void init_task_manager(task_manager_t* man, uint32_t num_threads)
{
  assert(man != NULL);
  assert(num_threads > 0 && num_threads < 33 && "Do you have zero or more than 32 processors??");

  man->q_arr = calloc(num_threads, sizeof(not_q_t));
  assert(man->q_arr != NULL && "Memory exhausted");
    
  for(uint32_t i = 0; i < num_threads; ++i){
    init_not_q(&man->q_arr[i]);   
  }

  man->t_arr = calloc(num_threads, sizeof(pthread_t));
  assert(man->t_arr != NULL && "Memory exhausted" );
  man->len_thr = num_threads;

  for(uint32_t i = 0; i < num_threads; ++i){
    task_thread_args_t* args = malloc(sizeof(task_thread_args_t) ); 
    args->idx = i;
    args->man = man;

    int rc = pthread_create(&man->t_arr[i], NULL, worker_thread, args);
    assert(rc == 0);
  }

  man->index = 0;
}

void free_task_manager(task_manager_t* man, void (*clean)(void*))
{
  for(uint32_t i = 0; i < man->len_thr; ++i){
    done_not_q(&man->q_arr[i]);
  }

  for(uint32_t i = 0; i < man->len_thr; ++i){
    pthread_join(man->t_arr[i], NULL); 
  }

  for(uint32_t i = 0; i < man->len_thr; ++i){
    free_not_q(&man->q_arr[i], clean); 
  }

  free(man->q_arr);

  free(man->t_arr);
}

void async_task_manager(task_manager_t* man, task_t t)
{
  assert(man != NULL);
  assert(t.func != NULL);
  //assert(t.args != NULL);

  uint64_t index = man->index++;
//  atomic_fetch_add_explicit(&man->index, 1, memory_order_relaxed);

  
  for(uint32_t i = 0; i < man->len_thr; ++i){
    if(try_push_not_q(&man->q_arr[(i+index) % man->len_thr], t)){
//      printf("taksed pushed into the queue id %ld with val = %d\n", (i+index) % man->len_thr, *(int*)t.args);
      return;
    }
  }

  push_not_q(&man->q_arr[index%man->len_thr], t);
}


/*
MIT License

Copyright (c) 2022 Mikel Irazabal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "thread_pool.h"

#include <assert.h>
#include <stdlib.h>

static
void* worker_thread_pool(void* arg)
{
  assert(arg != NULL);

  thread_pool_t* p = (thread_pool_t*)arg;

  while(true){
       
    pthread_mutex_lock(&p->mtx);

    while(p->stop_token == false && p->q.sz == 0){
      pthread_cond_wait(&p->cond , &p->mtx);
    }

    if(p->stop_token == true){
      pthread_mutex_unlock(&p->mtx);
      break;
    }

    void* v = dequeue_thread_pool_queue(&p->q); 

    pthread_mutex_unlock(&p->mtx);

    p->worker_func(v);

  } 

  return NULL;
}


void init_thread_pool(thread_pool_t* p, uint32_t num_threads, void (*work_func)(void*), void (*free_func)(void*) )
{
  assert(p != NULL);
  assert(work_func != NULL);
  assert(free_func != NULL);
  assert(num_threads < 16 && "Do you really want to use so many threads?");

  p->worker_func = work_func;
  p->free_func = free_func;
  p->num_threads = num_threads;

  init_thread_pool_queue(&p->q);

  pthread_mutexattr_t* attr = NULL;
#ifdef DEBUG
  pthread_mutexattr_settype(attr, PTHREAD_MUTEX_ERRORCHECK);
#endif
  int rc = pthread_mutex_init(&p->mtx, attr);
  assert(rc == 0 && "Error while creating the mtx");

  pthread_condattr_t* c_attr = NULL; 
  rc = pthread_cond_init(&p->cond, c_attr);
  assert(rc == 0);

  p->stop_token = false;

  p->t_arr = calloc(num_threads, sizeof(pthread_t));
  assert(p->t_arr != NULL && "Memory exhausted");

  for(int i = 0; i < num_threads; ++i){
    rc = pthread_create(&p->t_arr[i], NULL, worker_thread_pool, p);
    assert(rc == 0);
  }
}

void stop_thread_pool(thread_pool_t* p)
{
  assert(p != NULL);

  pthread_mutex_lock(&p->mtx);
  p->stop_token = true;
  pthread_mutex_unlock(&p->mtx);

  for(size_t i = 0; i < p->num_threads; ++i){
    pthread_join(p->t_arr[i], NULL);
  }
  free(p->t_arr);

  

  int rc = pthread_mutex_destroy(&p->mtx);
  assert(rc == 0);

  rc = pthread_cond_destroy(&p->cond);
  assert(rc == 0);
  
  free_thread_pool_queue(&p->q, p->free_func);
}

void add_value_thread_pool(thread_pool_t* p, void* value)
{
  assert(p != NULL);
  assert(value != NULL);

  pthread_mutex_lock(&p->mtx);

  enqueue_thread_pool_queue(&p->q, value);

  pthread_cond_signal(&p->cond);

  pthread_mutex_unlock(&p->mtx);
}


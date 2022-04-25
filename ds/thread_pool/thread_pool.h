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


/*
 * Minimal POSIX Thread Pool  
*/


#ifndef NAIVE_POSIX_THREAD_POOL_H
#define NAIVE_POSIX_THREAD_POOL_H 

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "thread_pool_queue.h"

typedef struct{
 
  void (*free_func)(void*);
  void (*worker_func)(void*);

  thread_pool_queue_t q;

  pthread_mutex_t mtx;
  pthread_cond_t cond;

  bool stop_token;

  uint32_t num_threads; 
  pthread_t* t_arr;

} thread_pool_t;

void init_thread_pool(thread_pool_t* p, uint32_t num_threads, void (*work_func)(void*), void (*free_func)(void*) );

void stop_thread_pool(thread_pool_t* p);

void add_value_thread_pool(thread_pool_t* p, void* value);

#endif


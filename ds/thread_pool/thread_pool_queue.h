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
 * Single Linked List for the Thread Pool
*/

#ifndef NAIVE_THREAD_POOL_QUEUE_H
#define NAIVE_THREAD_POOL_QUEUE_H 

#include <stddef.h>

typedef struct thread_pool_queue_node_s{
  struct thread_pool_queue_node_s* next;
  void* val;
} thread_pool_queue_node_t;

typedef struct{
 thread_pool_queue_node_t* head;
 thread_pool_queue_node_t* tail;
 size_t sz;
} thread_pool_queue_t;

void init_thread_pool_queue(thread_pool_queue_t* l);

void free_thread_pool_queue(thread_pool_queue_t* l, void (*free_func)(void*) );

void enqueue_thread_pool_queue(thread_pool_queue_t* l, void* v);

void* dequeue_thread_pool_queue(thread_pool_queue_t* l);

#endif


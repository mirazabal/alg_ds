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



#include "thread_pool_queue.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void init_thread_pool_queue(thread_pool_queue_t* l)
{
  assert(l != NULL);

  l->sz = 0;
  l->head = NULL;
  l->tail = NULL;
}

void free_thread_pool_queue(thread_pool_queue_t* l, void (*free_func)(void*) )
{
  assert(l != NULL);

  while(l->head != NULL){
    if(free_func != NULL)
      free_func(l->head->val);

    thread_pool_queue_node_t* it = l->head;
    l->head = l->head->next;
    free(it);
  }

}

void enqueue_thread_pool_queue(thread_pool_queue_t* l, void* v)
{
  assert(l != NULL);
  assert(v != NULL);

  thread_pool_queue_node_t* n = malloc(sizeof(thread_pool_queue_node_t));
  assert(n != NULL);
  n->next = NULL;
  n->val = v;

  if(l->tail != NULL)
    l->tail->next = n;
  l->tail = n;

  l->sz += 1;

  if(l->head == NULL)
    l->head = n;
}

void* dequeue_thread_pool_queue(thread_pool_queue_t* l)
{
  assert(l != NULL);
  if(l->sz == 0)
    return NULL;

  thread_pool_queue_node_t* it = l->head;

  void* v = l->head->val;
  l->head = l->head->next;
  free(it); 

  l->sz -= 1;
  return v;
}


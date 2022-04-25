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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

static
void do_work(void* arg)
{
  int* a = (int*)arg;

  int slp = rand()%64;
  usleep(slp);

  pid_t tid = syscall(__NR_gettid);
  printf("Thread tid = %d \n", tid);
  free(a);
}

static
void free_val(void* arg)
{
  int* a = (int*)arg;
  free(a);
}

int main()
{
  thread_pool_t p;
  init_thread_pool(&p, 8, do_work, free_val );

  srand(time(0));

  for(int i = 0; i < 1024*4096; ++i){
    int* a = malloc(sizeof(int));
    add_value_thread_pool(&p, a);
  }

  stop_thread_pool(&p);

  return EXIT_SUCCESS;
}


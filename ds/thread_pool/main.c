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
#include "task.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

int64_t time_now_us(void)
{
  struct timespec tms;

  /* The C11 way */
  /* if (! timespec_get(&tms, TIME_UTC))  */

  /* POSIX.1-2008 way */
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
    return -1;
  }
  /* seconds, multiplied with 1 million */
  int64_t micros = tms.tv_sec * 1000000;
  /* Add full microseconds */
  micros += tms.tv_nsec/1000;
  /* round up if necessary */
  if (tms.tv_nsec % 1000 >= 500) {
    ++micros;
  }
  return micros;
}

static
int64_t naive_fibonnacci(int64_t a)
{
  if(a < 2)
    return 1;
  
  return naive_fibonnacci(a-1) + naive_fibonnacci(a-2);
}

static
void do_work(void* arg)
{
  int64_t* a = (int64_t*)arg;
  naive_fibonnacci(30);

  //printf("time do_work %ld \n", time_now_us() );

//  pid_t tid = syscall(__NR_gettid);
//  printf("Thread tid = %d \n", tid);
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
  init_thread_pool(&p, 8);

  srand(time(0));

  printf("Starting %ld \n", time_now_us() );
  for(int i = 0; i < 64*1024; ++i){
    int64_t* a = malloc(sizeof(int64_t));
    *a =  time_now_us();
    task_t t  = { .func=do_work, .args = a };
    async_thread_pool(&p, t);
  }

  sleep(35); 
  printf("Stopping thread pool\n");
  stop_thread_pool(&p, free_val);

  return EXIT_SUCCESS;
}


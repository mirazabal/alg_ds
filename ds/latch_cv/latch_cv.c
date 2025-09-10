/*
MIT License

Copyright (c) 2024 Mikel Irazabal

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



#include "latch_cv.h"
#include "../lock_guard/lock_guard.h"
#include "stdio.h"
#include <assert.h>
#include <errno.h>

latch_cv_t init_latch_cv(size_t cnt)
{
  assert(cnt > 0);
  
  latch_cv_t dst = {.cnt = cnt }; 
  
  const pthread_mutexattr_t * attr = NULL;
  int rc = pthread_mutex_init(&dst.mtx, attr);
  assert(rc == 0);
  (void)rc;

  const pthread_condattr_t *attr2 = NULL;
  rc = pthread_cond_init(&dst.cond, attr2);
  assert(rc == 0);

  return dst;
}

void count_down_latch_cv(latch_cv_t* l)
{
  assert(l != NULL);
 
  atomic_fetch_sub(&l->cnt, 1);
  if(l->cnt <= 0){
    lock_guard(&l->mtx);
    pthread_cond_signal(&l->cond);
  }
}

int wait_timeout_latch_cv(latch_cv_t* l, int timeout_sec)
{
  assert(l != NULL);
  assert(timeout_sec > -1 && timeout_sec < 60 && "You really want to not wait or wait for more than a minute?");

  struct timespec ts = {0};
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += timeout_sec;

  lock_guard(&l->mtx);
  int rc = 0;
  while(l->cnt > 0 && rc == 0){
    rc = pthread_cond_timedwait(&l->cond, &l->mtx, &ts);
    assert(rc == 0 || rc == ETIMEDOUT);
  }
  return rc;
}

void wait_latch_cv(latch_cv_t* l)
{
  assert(l != NULL);

  lock_guard(&l->mtx);
  while(l->cnt != 0){
    int rc = pthread_cond_wait(&l->cond, &l->mtx);
    assert(rc == 0);
    (void)rc;
  }
}

void free_latch_cv(latch_cv_t* l)
{
  assert(l != NULL);

  int rc = pthread_cond_destroy(&l->cond);
  assert(rc == 0);

  rc = pthread_mutex_destroy(&l->mtx);
  assert(rc == 0);
  (void)rc;
}


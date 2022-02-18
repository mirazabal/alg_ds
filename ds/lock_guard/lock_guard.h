#ifndef MIR_LOCK_GUARD_H
#define MIR_LOCK_GUARD_H 

#include "../../alg/defer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


// ToDO: Have a look at the implementation on QEMU_LOCK_GUARD


void mir_dummy_lock_guard(){};
void mir_dummy_lock_guard_const(){};

// __attribute__((unused)) 

#define CHECK_TYPE_INLINE(val, type) _Generic((val), type: mir_dummy_lock_guard, const type: mir_dummy_lock_guard_const)(val)

#define lock_guard(X) \
  do{ CHECK_TYPE_INLINE(X, pthread_mutex_t*);\
     int rc = pthread_mutex_lock(X);  \
     if (rc != 0){ \
        fprintf(stdout, "Error while locking, possibly the lock is already locked: %s\n", strerror(rc)); \
        fflush(stdout); \
        assert(0!=0); \
        exit(-1); \
     } \
  } while(0); \
   defer( { int rc = pthread_mutex_unlock(X); \
         if(rc != 0){ \
          fprintf(stdout, "Error while unlocking: %s\n", strerror(rc) ); \
          fflush(stdout); \
          assert(0!=0); \
          exit(-1); \
         } \
         }); \

//         printf("Defering... \n" ); 

#endif


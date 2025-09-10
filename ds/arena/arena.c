/*
MIT License

Copyright (c) 2025 Mikel Irazabal

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



#include "arena.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sanitizer/asan_interface.h>


//#include "/usr/lib/gcc/aarch64-linux-gnu/11/include/sanitizer/asan_interface.h"

//#define ASAN_POISON_MEMORY_REGION(addr, size) \
//#define ASAN_UNPOISON_MEMORY_REGION(addr, size) \


#define ARENA_SIZE (1ul<<30)
#define ARENA_ALIGMENT (4)
#define ARENA_PAGE_SIZE (4096)
#define ARENA_COMMIT_MIN_MEMORY 16*ARENA_PAGE_SIZE

typedef struct{
  uint8_t* mem;
  uint64_t offset;
  uint64_t commited;
} arena_t;

void* init_arena(void)
{
  assert(sysconf(_SC_PAGESIZE) == ARENA_PAGE_SIZE);

  // ARENA_SIZE is only reserved not committed
  void* b = mmap(NULL, ARENA_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
  assert(b != MAP_FAILED);

  arena_t a = {0};
  a.mem = b;
  a.offset = sizeof(arena_t);

  // ARENA_COMMIT_MIN_MEMORY is committed into virtual memory
  int rc = mprotect(b, ARENA_COMMIT_MIN_MEMORY, PROT_READ | PROT_WRITE);
  assert(rc == 0);
  a.commited = ARENA_COMMIT_MIN_MEMORY;

  memcpy(b, &a, sizeof(arena_t));

  ASAN_POISON_MEMORY_REGION(b+sizeof(arena_t), a.commited - sizeof(arena_t));

  return b;
}

void free_arena(void* ctx)
{
  assert(ctx != NULL);

  arena_t* a = ctx;

  munmap(a, ARENA_SIZE);
}

static
void maybe_commit(arena_t* a)
{
  assert(a != NULL);

  if(a->offset > a->commited){
    uint64_t tmp = a->offset - a->commited;
    tmp = tmp / (ARENA_COMMIT_MIN_MEMORY);
    tmp += 1;

    uint64_t new_commit_sz = tmp * ARENA_COMMIT_MIN_MEMORY;
    int rc = mprotect(a->mem+a->commited, new_commit_sz , PROT_READ | PROT_WRITE);
    assert(rc == 0);
    a->commited += new_commit_sz;
  }
}

static
void maybe_head_align(arena_t* a, uint64_t align)
{
  assert(a != NULL);

  uint64_t addr = (uint64_t)(a->mem+a->offset);
  uint64_t rem = addr % align;
  if(rem != 0){
    a->offset += align - rem;
  }
  assert((uint64_t)(a->mem+a->offset)% align == 0 && "Alignment failed?");
}

static
void maybe_tail_align(arena_t* a, uint64_t sz)
{
  assert(a != NULL);

  ASAN_UNPOISON_MEMORY_REGION(a->mem + a->offset, sz);

  uint64_t rem = sz % ARENA_ALIGMENT;
  if(rem != 0){
    sz += ARENA_ALIGMENT - rem;
  }

  uint64_t poison_mem_bytes = 64;

  a->offset += sz + poison_mem_bytes;
}

void* alloc_arena(void* ctx, uint64_t sz, uint64_t align)
{
  assert(ctx != NULL);
  assert(__builtin_popcount(align) == 1);
  assert(align < 128 && "More than 64 bytes alignment needed?");
  align < ARENA_ALIGMENT ? ARENA_ALIGMENT : align;

  arena_t* a = ctx;
  assert(a->offset % ARENA_ALIGMENT == 0 && "Precondition");

  // Check head alignment
  maybe_head_align(a, align);

  void* p = a->mem+a->offset;

  // Check tail alignment
  maybe_tail_align(a, sz);

  // Commit memory
  maybe_commit(a);

  return p;
}

void reset_arena(void* ctx)
{
  assert(ctx != NULL);

  arena_t* a = ctx;

  a->offset = sizeof(arena_t);

  ASAN_POISON_MEMORY_REGION(a->mem+sizeof(arena_t), a->commited - sizeof(arena_t));
}


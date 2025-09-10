#include "arena.h"
#include <assert.h>
#include <stdalign.h>


typedef struct{
  uint64_t a;
  int b;
  char c;
  uint16_t d;
} foo_t;


int main()
{
  void* arena = init_arena();
  assert(arena != NULL);

  int* a = alloc_arena(arena, sizeof(int), alignof(int));

  *a = 5;
  assert(*a == 5);

  reset_arena(arena);

  int* b = alloc_arena(arena, sizeof(int), alignof(int));

  assert(*b == 5);
  *b = 7;
  assert(*b == 7);

  int* c = alloc_arena(arena, 64*1024*1024*sizeof(int), alignof(int));

  c[1024] = 9;

  reset_arena(arena);

  foo_t* f = alloc_arena(arena, 128*sizeof(foo_t), alignof(foo_t));

  f[0].a = 128;

  free_arena(arena);


  return 0;
}

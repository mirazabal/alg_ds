#include "accumulate.h"
#include <assert.h>

uint64_t accumulate_u32(uint32_t* start, uint32_t* end)
{
  uint64_t val = 0;
  void* start_v = start;
  void* end_v = end;
  while(start_v != end_v){
    val += *(uint32_t*)start_v;
    start_v += sizeof(uint32_t); 
  }
  return val;
}

uint64_t accumulate_ring(seq_ring_t* r, void* start_it, void* end_it)
{
  assert(r != NULL);
  assert(start_it != NULL);
  assert(end_it != NULL);
  assert(r->elt_size == sizeof(uint32_t));

  uint64_t val = 0;
  void* it = start_it;
  while(it != end_it){
    val += *(uint32_t*)it;
    it = seq_next(r, it );
  }

  return val;
}


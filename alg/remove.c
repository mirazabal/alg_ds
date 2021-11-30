#include "remove_if.h"

#include <assert.h>

seq_arr_t remove_if_range_reg(assoc_reg_t* reg, bool (*f)(void* data))
{
  assert(reg != NULL);
  assert(f != NULL);

  seq_arr_t arr = {0}; 

  seq_init(&arr, sizeof(uint32_t));

  const size_t b_size = bucket_sz(reg);
  for(int i =0; i < reg->occ_bucket; ++i){
    registry_bucket_t* b = reg->arr + b_size*i;
    if(b->has_value && f(b->val) == true){
      static_assert(sizeof( b->key ) == sizeof(uint32_t), "Data size mismatch");
      seq_push_back(&arr, &b->key, sizeof( b->key ));
      --reg->sz;
      b->has_value = false; 
    }
  } 

  compact_if_necessary(reg);
  shrink_if_necessary(reg);
  return arr;
}



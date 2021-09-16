#include "for_each.h"
#include <assert.h>

void for_each_arr(seq_arr_t* arr, void* start_it, void* end_it, void(*f)(const void*, const void*), void* data)
{
  assert(arr != NULL);
  assert(start_it != NULL);
  assert(end_it != NULL);
  for( ; start_it != end_it; start_it += arr->elt_size){
    f(start_it,data);
  }
}




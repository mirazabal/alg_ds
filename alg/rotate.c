#include "alg.h"
#include "rotate.h"

void* rotate(void* first_v, void* n_first, void* last_v, size_t elm_size)
{
  uint8_t* first = (uint8_t*)first_v; 
  uint8_t* last = (uint8_t*)last_v;  
  uint8_t* first_2 = (uint8_t*)n_first;

  do{
    iter_swap(first, first_2, elm_size);
    first += elm_size;
    first_2 += elm_size;
    if(first == n_first)
      n_first = first_2;
  } while(first_2 != last);

  void* ret = first;
  first_2 = n_first;
  
  while(first_2 != last){
    iter_swap(first, first_2,elm_size);
    first += elm_size;
    first_2 += elm_size;
    if(first == n_first)
      n_first = first_2;
    else if (first_2 == last)
      first_2 = n_first;
  }
  return ret;
}


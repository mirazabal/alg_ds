#include "find.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
/*
void* find(void* start_it, void* end_it, void* value, bool (*f)(const void*, const void*))
{
  while(start_it != end_it){
    if(f(value,start_it)){
      return start_it; 
    }  

    start_it = seq_next(l,start_it);
  }
  return NULL;
}
*/


void* find_if_list(const seq_list_t* l, void* start_it, void* end_it, void* value, bool (*f)(const void*, const void*))
{
  assert(0 != 0 && "not implemented");
  /*
  printf("Find sequence seq_size = %lu\n",l->size);
  while(start_it != end_it){
    if(f(value, seq_value(l,start_it))){
      printf("Value matched!!!\n");
      return start_it;
    }
    printf("Advancing iterator!\n");
    start_it = seq_next(l,start_it);
  }
  printf("Not Value matched!!!\n");
  return start_it; 
  */
}


void* find_if_arr(seq_arr_t* arr, void* start_it, void* end_it, void* value , bool(*f)(const void*, const void*))
{
  assert(arr != NULL);

  while( start_it != end_it){
    if(f(value,start_it)) 
      return start_it;
    start_it = seq_next(arr,start_it);
  }
  return start_it;
}

void* find_if_ring(seq_ring_t* arr, void* start_it, void* end_it, void* value , bool(*f)(const void*, const void*))
{
  assert(arr != NULL);
  while(start_it != end_it){
    if(f(value,start_it)) 
      return start_it;

    start_it = seq_next(arr,start_it);
  }
  return start_it;
}

void* find_if_rb_tree(assoc_rb_tree_t* tree, void* start_it, void* end_it, void const* value, bool(*f)(const void*, const void*))
{
  assert(tree != NULL);

  while(start_it != end_it){
    if(f(value, assoc_key(tree, start_it)))
      return start_it;
    start_it = assoc_next(tree, start_it);
  }

  return start_it;
}

bml_iter_t find_if_bi_map_left(bi_map_t* map, bml_iter_t start_it, bml_iter_t end_it, void const* value, bool(*f)(const void*, const void*))
{
  assert(map != NULL);
  assert(start_it.it != NULL);

  while(start_it.it != end_it.it){
    if(f(value, assoc_key(&map->left, start_it.it) ) )
      return start_it;
    start_it = bi_map_next_left(map, start_it);
  }

  return start_it;
}

bmr_iter_t find_if_bi_map_right(bi_map_t* map, bmr_iter_t start_it, bmr_iter_t end_it, void const* value, bool(*f)(const void*, const void*))
{
  assert(map != NULL);
  assert(start_it.it != NULL);

  while(start_it.it != end_it.it){
    if(f(value, start_it.it) == true)
      return start_it;

    start_it = bi_map_next_right(map, start_it);
  }

  return start_it;
}





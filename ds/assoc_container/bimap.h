#ifndef BIDIRECTIONAL_HASH_MAP
#define BIDIRECTIONAL_HASH_MAP

/*
 *  Naive bidirectional map. K1->K2 and K2->K1 
 */


#include "assoc_rb_tree.h"

typedef int (*bi_map_cmp)(void const*, void const*);

typedef void (*free_func_t)(void* key, void* value);

typedef struct
{
  assoc_rb_tree_t left;
  assoc_rb_tree_t right;
} bi_map_t;

typedef struct{
  void* it;
} bml_iter_t;

typedef struct{
  void* it;
} bmr_iter_t;

void bi_map_init(bi_map_t* map, size_t key_sz_1, size_t key_sz_2, bi_map_cmp cmp1, bi_map_cmp cmp2, free_func_t free1, free_func_t free2);

void bi_map_free(bi_map_t* map);

// Modifiers
void bi_map_insert(bi_map_t* map, void const* key1, size_t key_sz1, void const* key2, size_t key_sz2);


// It returns the void* of key2. the void* of the key1 is freed
void* bi_map_extract_left(bi_map_t* map, void* key1, size_t key1_sz);

// It returns the void* of key1. the void* of the key2 is freed
void* bi_map_extract_right(bi_map_t* map, void* key2, size_t key1_sz);


// returns a pointer to the value
void* bi_map_value_left(bi_map_t* map, bml_iter_t it);

// returns a pointer to the value
void* bi_map_value_right(bi_map_t* map, bml_iter_t it);

// Capacity
size_t bi_map_size(bi_map_t* map);



// Forward Iterator Concept

bml_iter_t bi_map_front_left(bi_map_t* map);

bml_iter_t bi_map_next_left(bi_map_t* map, bml_iter_t);

bml_iter_t bi_map_end_left(bi_map_t* map);

bmr_iter_t bi_map_front_right(bi_map_t* map);

bmr_iter_t bi_map_next_right(bi_map_t* map, bmr_iter_t);

bmr_iter_t bi_map_end_right(bi_map_t* map);



#endif


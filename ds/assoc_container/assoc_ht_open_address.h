#ifndef ASSOC_HASH_TABLE_OPEN_ADDRESSING
#define ASSOC_HASH_TABLE_OPEN_ADDRESSING 


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct{
  const void* key;
  void* value;
} kv_pair_t;

typedef struct{
  kv_pair_t kv;
  uint32_t hash;
  bool is_dirty;
  bool has_value;
} hentry_t;


//typedef uint32_t (*hash_func_fp)(const void* key);

typedef bool (*key_eq_func_fp)(const void* a, const void* b);

//typedef void (*free_key_value_func_fp)(kv_pair_t* kv);


typedef struct {
  hentry_t* arr;
  size_t key_sz;
  uint32_t sz;
  uint32_t cap;
  // # of used entries, even if they are now freed
  uint32_t num_dirty; 
  key_eq_func_fp key_eq;
} assoc_ht_open_t;


void assoc_ht_open_init(assoc_ht_open_t* htab, size_t key_sz, bool (*key_eq)(const void*, const void*));

void assoc_ht_open_free(assoc_ht_open_t* ht, void (*free_func)(void* key, void* value));

// Modifiers
// The tree is responsible for freeing the void* key and value memory later
void assoc_ht_open_insert(assoc_ht_open_t* ht, void const* key, size_t key_sz, void* value);

// It returns the void* of value. the void* of the key is freed
void* assoc_ht_open_extract(assoc_ht_open_t* ht, void* key);

// Get the key from an iterator 
void* assoc_ht_open_key(assoc_ht_open_t* ht, void* it);

// Get the value pointer form an iterator
void* assoc_ht_open_value(assoc_ht_open_t* ht, void* it);

// Capacity
size_t assoc_ht_open_size(assoc_ht_open_t* ht);

// Forward Iterator Concept
void* assoc_ht_open_front(assoc_ht_open_t const* ht);

void* assoc_ht_open_next(assoc_ht_open_t const* ht, void* it);

void* assoc_ht_open_end(assoc_ht_open_t const* ht);


#endif



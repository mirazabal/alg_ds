/*
MIT License

Copyright (c) 2021 Mikel Irazabal

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

#include "assoc_ht_open_address.h"

#include "assoc_ht_open_address.h"
#include "../../alg/murmur_hash_32.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

static 
const int MIN_SIZE_HT = 16;

static inline
uint32_t hash_func(const void* key_v)
{
  int32_t* key = (int32_t*)key_v;
  static const uint32_t seed = 42;
  return murmur3_32((uint8_t*)key, sizeof(uint32_t), seed);
}

static
uint32_t find_idx(assoc_ht_open_t* htab, const void* key, uint32_t hash)
{
  const uint32_t start_idx = hash % htab->cap;
  uint32_t idx = start_idx;
  do{
    const hentry_t* entry = &htab->arr[idx];
    if(entry->is_dirty == false)
      return idx;

    if(entry->has_value == true && htab->key_eq(key, entry->kv.key) == true){
      return idx;
    }

    idx += 1;
    if(idx == htab->cap)
      idx = 0;
  } while(start_idx != idx);

  assert(0!=0 && "Impossible code path. There must be a free entry.");
}

static
void rehash_table(assoc_ht_open_t* htab, uint32_t cap)
{
  assert(htab != NULL);

  assoc_ht_open_t old_htab = *htab;

  hentry_t* new_arr = calloc(cap, sizeof(hentry_t)); 
  assert(new_arr != NULL);

  htab->cap = cap;
  htab->arr = new_arr;
  htab->num_dirty = 0;

  for(size_t idx_old = 0; idx_old < old_htab.cap; ++idx_old ){
    if(old_htab.arr[idx_old].has_value == true){
      assert(old_htab.arr[idx_old].is_dirty == true);
      const uint32_t hash = old_htab.arr[idx_old].hash;
      const void* key = old_htab.arr[idx_old].kv.key;
      const uint32_t idx_new = find_idx(htab, key, hash); 
      htab->arr[idx_new] = old_htab.arr[idx_old];
      htab->num_dirty += 1;
    }
  }
  free(old_htab.arr);
}

static
void expand_or_shrink_if_neccesary(assoc_ht_open_t* htab)
{
  if(htab->num_dirty * 2 > htab->cap){
      rehash_table(htab, htab->cap * 2);
  } else if (htab->sz * 4 < htab->cap && htab->cap > MIN_SIZE_HT){
      rehash_table(htab, htab->cap/2);
  } 
  assert(htab->sz * 2 <= htab->cap);
  assert(htab->num_dirty * 2 <= htab->cap);
}

void assoc_ht_open_init(assoc_ht_open_t* htab, size_t key_sz, bool (*key_eq)(const void*, const void*))
//void init_htab(htab_t* htab, hash_func_fp hf, key_eq_func_fp key_eq, free_key_value_func_fp free_kv)
{
  assert(htab != NULL);
  assert(key_sz > 0);

  htab->arr = calloc(MIN_SIZE_HT, sizeof(hentry_t));
  assert(htab->arr != NULL);
  htab->cap = MIN_SIZE_HT;
  htab->sz = 0;
  htab->num_dirty = 0;
  htab->key_eq = key_eq;
}

void assoc_ht_open_free(assoc_ht_open_t* htab, void (*free_func)(void* key, void* value))
{
  assert(htab != NULL);

    for(uint32_t i = 0; i < htab->cap; ++i){
      if(htab->arr[i].has_value == true){
        assert(htab->arr[i].is_dirty);
        free_func(&htab->arr[i].kv.key, &htab->arr[i].kv.value);
      }
    }
  free(htab->arr);
}

void assoc_ht_open_insert(assoc_ht_open_t* htab, void const* key, size_t key_sz, void* value)
//void insert_kv_htab(htab_t* htab, const void* key, void* value)
{
  assert(htab != NULL);
  assert(key != NULL);
  assert(value != NULL);

  assert(htab->key_sz == key_sz);

  expand_or_shrink_if_neccesary(htab);
  assert(htab->num_dirty * 2 <= htab->cap );

  const uint32_t hash = hash_func(key);
  const uint32_t idx = find_idx(htab, key, hash);
  assert(idx < htab->cap); 

  hentry_t* entry = &htab->arr[idx];
  if(entry->has_value == true){
    assert(entry->is_dirty == true);
    htab->free_kv(&entry->kv);
  } else
    htab->sz += 1;

  entry->kv.key = key;
  entry->kv.value = value;
  entry->hash = hash;
  if(entry->is_dirty == false)
    htab->num_dirty += 1;
  entry->is_dirty = true;
  entry->has_value = true;

}

static
hentry_t* find_entry(assoc_ht_open_t* htab, const void* key)
{
  const uint32_t hash = hash_func(key);
  const uint32_t start_idx = hash % htab->cap;
  uint32_t idx = start_idx;

  do{
    hentry_t* entry = &htab->arr[idx];
    if(entry->is_dirty == false){
      assert(entry->has_value == false);
      return NULL;
    }
    if(entry->has_value && htab->key_eq(key, entry->kv.key) == true){
      return entry;
    }
    idx += 1;
    if(idx == htab->cap)
      idx = 0;

  } while(idx != start_idx);

  assert(0!=0 && "Impossible code path. There must be a free entry.");
}

void remove_value_htab(assoc_ht_open_t* htab, const void* key)
{
  expand_or_shrink_if_neccesary(htab);
  assert(htab->num_dirty * 2 <= htab->cap );

  hentry_t* entry = find_entry(htab, key);
  if(entry == NULL) return;

  assert(entry->is_dirty == true && entry->has_value == true);
   
  htab->free_kv(&entry->kv);
  entry->has_value = false;
  htab->sz -=1;
}

void* find_value_htab(assoc_ht_open_t* htab, const void* key)
{
  assert(htab != NULL);
  assert(key != NULL);
  hentry_t* entry = find_entry(htab, key); 

  return entry != NULL ? &entry->kv.value : NULL;
}






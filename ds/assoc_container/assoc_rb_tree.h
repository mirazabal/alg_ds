#ifndef ASSOCIATIVE_RB_TREE_NAIVE
#define ASSOCIATIVE_RB_TREE_NAIVE

/*
    RB tree implemented a la CLRS

    Red-Black trees have 5 properties:
    1- Every node is Red or Black
    2- The root is black
    3- Every leaf is black
    4- If a node is red, then both its children are black
    5- For each node, all simple paths from the node to descendant leaves contain the same number of black nodes 

    Lemma 1: a red-black tree with n internal nodes has a height at most 2lg(n+1)
*/

#include <stdbool.h>
#include <stddef.h>

typedef struct assoc_node_s assoc_node_t;

typedef void (*free_func_t)(void* key, void* value);

typedef struct{
  assoc_node_t* dummy; // node
  assoc_node_t* root; // node
  int (*comp)(const void*, const void*);
  size_t size;
  // size key
  size_t key_sz;
  free_func_t free_func;
  // size value
  // size_t val_sz; // Value is always a pointer to the type
  
} assoc_rb_tree_t;


void assoc_rb_tree_init(assoc_rb_tree_t* tree, size_t key_sz, int (*comp)(const void*, const void*), free_func_t f);

void assoc_rb_tree_free(assoc_rb_tree_t* tree);

// Modifiers
// The tree is responsible for freeing the void* key and value memory later
void assoc_rb_tree_insert(assoc_rb_tree_t* tree, void const* key, size_t key_sz, void* value);

// It returns the void* of value. the void* of the key is freed
void* assoc_rb_tree_extract(assoc_rb_tree_t* tree, void* key);

// Get the key from an iterator 
void* assoc_rb_tree_key(assoc_rb_tree_t* tree, void* it);

// Get the value pointer form an iterator
void* assoc_rb_tree_value(assoc_rb_tree_t* tree, void* it);

// Capacity
size_t assoc_rb_tree_size(assoc_rb_tree_t* tree);

// Forward Iterator Concept
void* assoc_rb_tree_front(assoc_rb_tree_t const* treea);

void* assoc_rb_tree_next(assoc_rb_tree_t const* tree, void* it);

void* assoc_rb_tree_end(assoc_rb_tree_t const* tree);

#endif


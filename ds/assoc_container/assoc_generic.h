#ifndef ASSOCCIATIVE_CONTAINERS_GENERIC
#define ASSOCCIATIVE_CONTAINERS_GENERIC

#include "assoc_rb_tree.h" 


// Initialization
#define assoc_init(T, KS, C, F) _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_init, \
                                        default:  assoc_rb_tree_init) (T, KS, C, F)

#define assoc_free(T) _Generic ((T), assoc_rb_tree_t*:  assoc_rb_tree_free, \
                                       default:   assoc_rb_tree_free)(T)


// Modifiers
#define assoc_insert(T, K, KS, V) _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_insert, \
                                       default:   assoc_rb_tree_insert)(T,K,KS,V)


#define assoc_extract(T, K)  _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_extract, \
                                       default:   assoc_rb_tree_extract)(T,K)

#define assoc_key(T,U)  _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_key, \
                                       default:   assoc_rb_tree_key)(T,U)

#define assoc_value(T,U)  _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_value, \
                                       default:   assoc_rb_tree_value)(T,U)


// Capacity
#define assoc_size(T) _Generic ((T), assoc_rb_tree_t*:  assoc_rb_tree_size, \
                                       default: assoc_rb_tree_size)(T)

// Forward Iterator Concept
#define assoc_front(T) _Generic ((T), assoc_rb_tree_t*: assoc_rb_tree_front, \
                                        default: assoc_rb_tree_front)(T)

#define assoc_next(T,U) _Generic((T), assoc_rb_tree_t*: assoc_rb_tree_next, \
                                        default:  assoc_rb_tree_next)(T,U)

#define assoc_end(T) _Generic((T),  assoc_rb_tree_t*: assoc_rb_tree_end, \
                                        default: assoc_rb_tree_end)(T)



#endif


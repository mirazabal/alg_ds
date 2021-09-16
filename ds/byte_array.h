#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  size_t len;
  uint8_t *buf;
} byte_array_t;

#define BYTE_ARRAY_STACK(name, length)  \
  uint8_t (name##_BuF)[(length)];       \
  memset((name##_BuF), 0, sizeof((name##_BuF))); \
  byte_array_t (name) = {.buf = (name##_BuF), .len = (length)}


byte_array_t copy_byte_array(byte_array_t src);

void free_byte_array(byte_array_t ba);

bool eq_byte_array(const byte_array_t* m0, const byte_array_t* m1);

#endif

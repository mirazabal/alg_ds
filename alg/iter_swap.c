#include "iter_swap.h"

#include <string.h>
#include <stdint.h>

void iter_swap(void* it_1, void* it_2, size_t elm_size)
{
  uint8_t buf[elm_size];
  memcpy(&buf, it_1, elm_size);
  memcpy(it_1, it_2, elm_size);
  memcpy(it_2, &buf, elm_size);
}


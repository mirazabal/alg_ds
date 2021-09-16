#include "byte_array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

byte_array_t copy_byte_array(byte_array_t src)
{
  byte_array_t dst;
  memset(&dst,0,sizeof(byte_array_t));
  dst.buf = malloc(src.len);
  memcpy(dst.buf,src.buf,src.len);
  dst.len = src.len;
  return dst;
}

void free_byte_array(byte_array_t ba)
{
  free(ba.buf);
}

bool eq_byte_array(const byte_array_t* m0, const byte_array_t* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->len != m1->len)
    return false;

  const int rc = memcmp(m0->buf, m1->buf, m0->len); 
  if(rc != 0)
    return false;

  return true;
}


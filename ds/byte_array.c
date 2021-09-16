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


 /*
  * Copyright (C) 2015 Southern Storm Software, Pty Ltd.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a
  * copy of this software and associated documentation files (the "Software"),
  * to deal in the Software without restriction, including without limitation
  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included
  * in all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  * DEALINGS IN THE SOFTWARE.
  */
  
 #include "Crypto.h"
  
 void clean(void *dest, size_t size)
 {
     // Force the use of volatile so that we actually clear the memory.
     // Otherwise the compiler might optimise the entire contents of this
     // function away, which will not be secure.
     volatile uint8_t *d = (volatile uint8_t *)dest;
     while (size > 0) {
         *d++ = 0;
         --size;
     }
 }
  
 bool secure_compare(const void *data1, const void *data2, size_t len)
 {
     uint8_t result = 0;
     const uint8_t *d1 = (const uint8_t *)data1;
     const uint8_t *d2 = (const uint8_t *)data2;
     while (len > 0) {
         result |= (*d1++ ^ *d2++);
         --len;
     }
     return (bool)((((uint16_t)0x0100) - result) >> 8);
 }
  
 uint8_t crypto_crc8(uint8_t tag, const void *data, unsigned size)
 {
     const uint8_t *d = (const uint8_t *)data;
     uint8_t crc = 0xFF ^ tag;
     uint8_t bit;
     while (size > 0) {
         crc ^= *d++;
         for (bit = 0; bit < 8; ++bit) {
             // if (crc & 0x80)
             //     crc = (crc << 1) ^ 0x1D;
             // else
             //     crc = (crc << 1);
             uint8_t generator = (uint8_t)((((int8_t)crc) >> 7) & 0x1D);
             crc = (crc << 1) ^ generator;
         }
         --size;
     }
     return crc;
 }
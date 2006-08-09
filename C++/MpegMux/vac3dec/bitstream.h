/*

  Bitstream class
  Bit-level data extraction from an input stream.

*/


#ifndef BITSTREAM_H              
#define BITSTREAM_H

#include "defs.h"

class Bitstream
{
private:
  uint32_t *buffer_start;
  uint32_t  bits_left;
  uint32_t  current_word;

  inline void fill_current();
  uint32_t get_bh  (uint32_t num_bits);
  int32_t  get_bh_signed(uint32_t num_bits);

public:

  void set_ptr (uint8_t * buf);

  inline uint32_t get(uint32_t num_bits);
  inline int32_t  get_signed(uint32_t num_bits);
//  inline bool     get_bool();
};








///////////////////////////////////////////////////////////////////////////////
// Inline
//


#ifdef WORDS_BIGENDIAN
#        define swab32(x) (x)
#else
#        if 0 && defined (__i386__)
#    define swab32(x) __i386_swab32(x)
     static inline const uint32_t __i386_swab32(uint32_t x)
     {
       __asm__("bswap %0" : "=r" (x) : "0" (x));
       return x;
     }
#  else
#    define swab32(x)\
        ((((uint8_t*)&x)[0] << 24) | (((uint8_t*)&x)[1] << 16) |  \
        (((uint8_t*)&x)[2] << 8)  | (((uint8_t*)&x)[3]))

#  endif
#endif



inline void 
Bitstream::fill_current()
{
  uint32_t tmp;
  tmp = *(buffer_start++);
  current_word = swab32(tmp);
}


inline uint32_t 
Bitstream::get(uint32_t num_bits)
{
  uint32_t result;

  if(num_bits < bits_left) 
  {
    result = (current_word << (32 - bits_left)) >> (32 - num_bits);
    bits_left -= num_bits;
    return result;
  }

  return get_bh(num_bits);
}

inline int32_t 
Bitstream::get_signed(uint32_t num_bits)
{
  int32_t result;
        
  if(num_bits < bits_left) 
  {
    result = (((int32_t)current_word) << (32 - bits_left)) >> (32 - num_bits);
    bits_left -= num_bits;
    return result;
  }

  return get_bh_signed(num_bits);
}
/*
inline bool
Bitstream::get_bool()
{
  bool result;
        
  if(bits_left) 
  {
    bits_left--;
    result = (current_word >> bits_left) & 1;
    return result;
  }

  fill_current();
  bits_left = 31;
  return (current_word >> 31) & 1;
}
*/

#endif

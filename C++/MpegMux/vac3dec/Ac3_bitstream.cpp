#include "bitstream.h"

void 
Bitstream::set_ptr (uint8_t * buf)
{
  int align;

  align = (long)buf & 3;
  buffer_start = (uint32_t *) (buf - align);
  bits_left = 0;
  get(align * 8);
}

uint32_t
Bitstream::get_bh(uint32_t num_bits)
{
  uint32_t result;

  num_bits -= bits_left;
  result = ((current_word << (32 - bits_left)) >> (32 - bits_left));

  fill_current();

  if(num_bits != 0)
    result = (result << num_bits) | (current_word >> (32 - num_bits));

  bits_left = 32 - num_bits;

  return result;
}

int32_t
Bitstream::get_bh_signed(uint32_t num_bits)
{
  int32_t result;

  num_bits -= bits_left;
  result = ((((int32_t)current_word) << (32 - bits_left)) >> (32 - bits_left));

  fill_current();

  if(num_bits != 0)
    result = (result << num_bits) | (current_word >> (32 - num_bits));
        
  bits_left = 32 - num_bits;

  return result;
}

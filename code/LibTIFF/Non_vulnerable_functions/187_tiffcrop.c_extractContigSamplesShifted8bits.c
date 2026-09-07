static int
extractContigSamplesShifted8bits (uint8 *in, uint8 *out, uint32 cols,
                                  tsample_t sample, uint16 spp, uint16 bps, 
			          tsample_t count, uint32 start, uint32 end,
 	                          int shift)
  {
  int    ready_bits = 0, sindex = 0;
  uint32 col, src_byte, src_bit, bit_offset;
  uint8  maskbits = 0, matchbits = 0;
  uint8  buff1 = 0, buff2 = 0;
  uint8 *src = in;
  uint8 *dst = out;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("extractContigSamplesShifted8bits","Invalid input or output buffer");
    return (1);
    }
  if ((start > end) || (start > cols))
    {
    TIFFError ("extractContigSamplesShifted8bits", 
               "Invalid start column value %d ignored", start);
    start = 0;
    }
  if ((end == 0) || (end > cols))
    {
    TIFFError ("extractContigSamplesShifted8bits", 
               "Invalid end column value %d ignored", end);
    end = cols;
    }
  ready_bits = shift;
  maskbits =  (uint8)-1 >> ( 8 - bps);
  buff1 = buff2 = 0;
  for (col = start; col < end; col++)
    {    
    bit_offset = col * bps * spp;
    for (sindex = sample; (sindex < spp) && (sindex < (sample + count)); sindex++)
      {
      if (sindex == 0)
        {
        src_byte = bit_offset / 8;
        src_bit  = bit_offset % 8;
        }
      else
        {
        src_byte = (bit_offset + (sindex * bps)) / 8;
        src_bit  = (bit_offset + (sindex * bps)) % 8;
        }
      src = in + src_byte;
      matchbits = maskbits << (8 - src_bit - bps); 
      buff1 = ((*src) & matchbits) << (src_bit);
      if ((col == start) && (sindex == sample))
        buff2 = *src & ((uint8)-1) << (shift);
      
      if (ready_bits >= 8)
        {
        *dst++ |= buff2;
        buff2 = buff1;
        ready_bits -= 8;
        }
      else
	buff2 = buff2 | (buff1 >> ready_bits);
      ready_bits += bps;
      }
    }
  while (ready_bits > 0)
    {
    buff1 = (buff2 & ((unsigned int)255 << (8 - ready_bits)));
    *dst++ = buff1;
    ready_bits -= 8;
    }
  return (0);
  } 

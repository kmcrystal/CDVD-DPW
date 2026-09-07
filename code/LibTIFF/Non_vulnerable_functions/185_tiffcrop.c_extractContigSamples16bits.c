static int
extractContigSamples16bits (uint8 *in, uint8 *out, uint32 cols, 
                            tsample_t sample, uint16 spp, uint16 bps, 
                            tsample_t count, uint32 start, uint32 end)
  {
  int    ready_bits = 0, sindex = 0;
  uint32 col, src_byte, src_bit, bit_offset;
  uint16 maskbits = 0, matchbits = 0;
  uint16 buff1 = 0, buff2 = 0;
  uint8  bytebuff = 0;
  uint8 *src = in;
  uint8 *dst = out;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("extractContigSamples16bits","Invalid input or output buffer");
    return (1);
    }
  if ((start > end) || (start > cols))
    {
    TIFFError ("extractContigSamples16bits", 
               "Invalid start column value %d ignored", start);
    start = 0;
    }
  if ((end == 0) || (end > cols))
    {
    TIFFError ("extractContigSamples16bits", 
               "Invalid end column value %d ignored", end);
    end = cols;
    }
  ready_bits = 0;
  maskbits = (uint16)-1 >> (16 - bps);
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
      matchbits = maskbits << (16 - src_bit - bps); 
      if (little_endian)
        buff1 = (src[0] << 8) | src[1];
      else
        buff1 = (src[1] << 8) | src[0];
      buff1 = (buff1 & matchbits) << (src_bit);
      if (ready_bits < 8) 
        { 
        bytebuff = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      else 
        {
        bytebuff = (buff2 >> 8);
        *dst++ = bytebuff;
        ready_bits -= 8;
        
        buff2 = ((buff2 << 8) | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  
  while (ready_bits > 0)
    {
    bytebuff = (buff2 >> 8);
    *dst++ = bytebuff;
    ready_bits -= 8;
    }
  return (0);
  } 

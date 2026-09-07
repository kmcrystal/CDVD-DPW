static int
extractContigSamplesShifted24bits (uint8 *in, uint8 *out, uint32 cols,
 	                           tsample_t sample, uint16 spp, uint16 bps, 
                                   tsample_t count, uint32 start, uint32 end,
	                           int shift)
  {
  int    ready_bits = 0, sindex = 0;
  uint32 col, src_byte, src_bit, bit_offset;
  uint32 maskbits = 0, matchbits = 0;
  uint32 buff1 = 0, buff2 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0;
  uint8 *src = in;
  uint8 *dst = out;
  if ((in == NULL) || (out == NULL))
    {
    TIFFError("extractContigSamplesShifted24bits","Invalid input or output buffer");
    return (1);
    }
  if ((start > end) || (start > cols))
    {
    TIFFError ("extractContigSamplesShifted24bits", 
               "Invalid start column value %d ignored", start);
    start = 0;
    }
  if ((end == 0) || (end > cols))
    {
    TIFFError ("extractContigSamplesShifted24bits", 
               "Invalid end column value %d ignored", end);
    end = cols;
    }
  ready_bits = shift;
  maskbits =  (uint32)-1 >> ( 32 - bps);
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
      matchbits = maskbits << (32 - src_bit - bps); 
      if (little_endian)
	buff1 = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3];
      else
	buff1 = (src[3] << 24) | (src[2] << 16) | (src[1] << 8) | src[0];
      if ((col == start) && (sindex == sample))
        buff2 = buff1 & ((uint32)-1) << (16 - shift);
      buff1 = (buff1 & matchbits) << (src_bit);
      if (ready_bits < 16)  
        {
        bytebuff1 = bytebuff2 = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      else 
        {
        bytebuff1 = (buff2 >> 24);
        *dst++ = bytebuff1;
        bytebuff2 = (buff2 >> 16);
        *dst++ = bytebuff2;
        ready_bits -= 16;
        
        buff2 = ((buff2 << 16) | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  
  while (ready_bits > 0)
    {
    bytebuff1 = (buff2 >> 24);
    *dst++ = bytebuff1;
    buff2 = (buff2 << 8);
    bytebuff2 = bytebuff1;
    ready_bits -= 8;
    }
  return (0);
  } 

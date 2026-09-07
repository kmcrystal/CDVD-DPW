static int
rotateContigSamples16bits(uint16 rotation, uint16 spp, uint16 bps, uint32 width, 
                         uint32 length,   uint32 col, uint8 *src, uint8 *dst)
  {
  int      ready_bits = 0;
  uint32   row, rowsize, bit_offset;
  uint32   src_byte = 0, src_bit = 0;
  uint16   matchbits = 0, maskbits = 0;
  uint16   buff1 = 0, buff2 = 0;
  uint8    bytebuff = 0;
  uint8   *next;
  tsample_t sample;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("rotateContigSamples16bits","Invalid src or destination buffer");
    return (1);
    }
  rowsize = ((bps * spp * width) + 7) / 8;
  ready_bits = 0;
  maskbits =  (uint16)-1 >> (16 - bps);
  buff1 = buff2 = 0;
  for (row = 0; row < length; row++)
    {
    bit_offset = col * bps * spp;
    for (sample = 0; sample < spp; sample++)
      {
      if (sample == 0)
        {
        src_byte = bit_offset / 8;
        src_bit  = bit_offset % 8;
        }
      else
        {
        src_byte = (bit_offset + (sample * bps)) / 8;
        src_bit  = (bit_offset + (sample * bps)) % 8;
        }
      switch (rotation)
	{
        case  90: next = src + src_byte - (row * rowsize);
                  break;
        case 270: next = src + src_byte + (row * rowsize);
	          break;
	default:  TIFFError("rotateContigSamples8bits", "Invalid rotation %d", rotation);
                  return (1);
        }
      matchbits = maskbits << (16 - src_bit - bps); 
      if (little_endian)
        buff1 = (next[0] << 8) | next[1];
      else
        buff1 = (next[1] << 8) | next[0];
      buff1 = (buff1 & matchbits) << (src_bit);
      
      if (ready_bits >= 8)
        {
        bytebuff = (buff2 >> 8);
        *dst++ = bytebuff;
        ready_bits -= 8;
        
        buff2 = ((buff2 << 8) | (buff1 >> ready_bits));
        }
      else
        { 
        bytebuff = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  if (ready_bits > 0)
    {
    bytebuff = (buff2 >> 8);
    *dst++ = bytebuff;
    }
  return (0);
  }  

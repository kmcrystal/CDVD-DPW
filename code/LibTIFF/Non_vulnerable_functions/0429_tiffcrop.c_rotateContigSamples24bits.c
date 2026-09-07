static int
rotateContigSamples24bits(uint16 rotation, uint16 spp, uint16 bps, uint32 width, 
                          uint32 length,   uint32 col, uint8 *src, uint8 *dst)
  {
  int      ready_bits = 0;
  uint32   row, rowsize, bit_offset;
  uint32   src_byte = 0, src_bit = 0;
  uint32   matchbits = 0, maskbits = 0;
  uint32   buff1 = 0, buff2 = 0;
  uint8    bytebuff1 = 0, bytebuff2 = 0;
  uint8   *next;
  tsample_t sample;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("rotateContigSamples24bits","Invalid src or destination buffer");
    return (1);
    }
  rowsize = ((bps * spp * width) + 7) / 8;
  ready_bits = 0;
  maskbits =  (uint32)-1 >> (32 - bps);
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
      matchbits = maskbits << (32 - src_bit - bps); 
      if (little_endian)
	buff1 = (next[0] << 24) | (next[1] << 16) | (next[2] << 8) | next[3];
      else
	buff1 = (next[3] << 24) | (next[2] << 16) | (next[1] << 8) | next[0];
      buff1 = (buff1 & matchbits) << (src_bit);
      
      if (ready_bits >= 16)
        {
        bytebuff1 = (buff2 >> 24);
        *dst++ = bytebuff1;
        bytebuff2 = (buff2 >> 16);
        *dst++ = bytebuff2;
        ready_bits -= 16;
        
        buff2 = ((buff2 << 16) | (buff1 >> ready_bits));
        }
      else
        { 
        bytebuff1 = bytebuff2 = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
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

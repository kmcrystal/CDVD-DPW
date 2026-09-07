static int
rotateContigSamples32bits(uint16 rotation, uint16 spp, uint16 bps, uint32 width, 
                          uint32 length,   uint32 col, uint8 *src, uint8 *dst)
  {
  int    ready_bits = 0 ;
  
  uint32 row, rowsize, bit_offset;
  uint32 src_byte, src_bit;
  uint32 longbuff1 = 0, longbuff2 = 0;
  uint64 maskbits = 0, matchbits = 0;
  uint64 buff1 = 0, buff2 = 0, buff3 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0, bytebuff3 = 0, bytebuff4 = 0;
  uint8   *next;
  tsample_t sample;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("rotateContigSamples24bits","Invalid src or destination buffer");
    return (1);
    }
  
  
  
  
  
  
  rowsize = ((bps * spp * width) + 7) / 8;
  ready_bits = 0;
  maskbits =  (uint64)-1 >> (64 - bps);
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
      matchbits = maskbits << (64 - src_bit - bps); 
      if (little_endian)
        {
	longbuff1 = (next[0] << 24) | (next[1] << 16) | (next[2] << 8) | next[3];
        longbuff2 = longbuff1;
        }
      else
        {
	longbuff1 = (next[3] << 24) | (next[2] << 16) | (next[1] << 8) | next[0];
        longbuff2 = longbuff1;
	}
      buff3 = ((uint64)longbuff1 << 32) | longbuff2;
      buff1 = (buff3 & matchbits) << (src_bit);
      if (ready_bits < 32)
        { 
        bytebuff1 = bytebuff2 = bytebuff3 = bytebuff4 = 0;
        buff2 = (buff2 | (buff1 >> ready_bits));
        }
      else 
        {
        bytebuff1 = (buff2 >> 56);
        *dst++ = bytebuff1;
        bytebuff2 = (buff2 >> 48);
        *dst++ = bytebuff2;
        bytebuff3 = (buff2 >> 40);
        *dst++ = bytebuff3;
        bytebuff4 = (buff2 >> 32);
        *dst++ = bytebuff4;
        ready_bits -= 32;
        
        buff2 = ((buff2 << 32) | (buff1 >> ready_bits));
        }
      ready_bits += bps;
      }
    }
  while (ready_bits > 0)
    {
    bytebuff1 = (buff2 >> 56);
    *dst++ = bytebuff1;
    buff2 = (buff2 << 8);
    ready_bits -= 8;
    }
  return (0);
  } 

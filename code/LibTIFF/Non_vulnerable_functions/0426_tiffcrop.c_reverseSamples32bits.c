static int
reverseSamples32bits (uint16 spp, uint16 bps, uint32 width, 
                      uint8 *ibuff, uint8 *obuff)
  {
  int    ready_bits = 0 ;
  
  uint32 bit_offset;
  uint32 src_byte = 0, high_bit = 0;
  uint32 col;
  uint32 longbuff1 = 0, longbuff2 = 0;
  uint64 mask_bits = 0, match_bits = 0;
  uint64 buff1 = 0, buff2 = 0, buff3 = 0;
  uint8  bytebuff1 = 0, bytebuff2 = 0, bytebuff3 = 0, bytebuff4 = 0;
  unsigned char *src;
  unsigned char *dst;
  tsample_t sample;
  if ((ibuff == NULL) || (obuff == NULL))
    {
    TIFFError("reverseSamples32bits","Invalid image or work buffer");
    return (1);
    }
  ready_bits = 0;
  mask_bits =  (uint64)-1 >> (64 - bps);
  dst = obuff;
  
  
  
  
  
  
  for (col = width; col > 0; col--)
    {
    
    bit_offset = (col - 1) * bps * spp;
    for (sample = 0; sample < spp; sample++)
      {
      if (sample == 0)
        {
        src_byte = bit_offset / 8;
        high_bit  = bit_offset % 8;
        }
      else
        {
        src_byte = (bit_offset + (sample * bps)) / 8;
        high_bit  = (bit_offset + (sample * bps)) % 8;
        }
      src = ibuff + src_byte;
      match_bits = mask_bits << (64 - high_bit - bps); 
      if (little_endian)
        {
	longbuff1 = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3];
        longbuff2 = longbuff1;
        }
      else
        {
	longbuff1 = (src[3] << 24) | (src[2] << 16) | (src[1] << 8) | src[0];
        longbuff2 = longbuff1;
	}
      buff3 = ((uint64)longbuff1 << 32) | longbuff2;
      buff1 = (buff3 & match_bits) << (high_bit);
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

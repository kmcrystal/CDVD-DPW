static int
reverseSamples24bits (uint16 spp, uint16 bps, uint32 width, 
                      uint8 *ibuff, uint8 *obuff)
  {
  int      ready_bits = 0;
  uint32   col;
  uint32   src_byte = 0, high_bit = 0;
  uint32   bit_offset = 0;
  uint32   match_bits = 0, mask_bits = 0;
  uint32   buff1 = 0, buff2 = 0;
  uint8    bytebuff1 = 0, bytebuff2 = 0;
  unsigned char *src;
  unsigned char *dst;
  tsample_t sample;
  if ((ibuff == NULL) || (obuff == NULL))
    {
    TIFFError("reverseSamples24bits","Invalid image or work buffer");
    return (1);
    }
  ready_bits = 0;
  mask_bits =  (uint32)-1 >> (32 - bps);
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
      match_bits = mask_bits << (32 - high_bit - bps); 
      if (little_endian)
	buff1 = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3];
      else
	buff1 = (src[3] << 24) | (src[2] << 16) | (src[1] << 8) | src[0];
      buff1 = (buff1 & match_bits) << (high_bit);
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

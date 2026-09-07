static int
reverseSamples16bits (uint16 spp, uint16 bps, uint32 width, 
                      uint8 *ibuff, uint8 *obuff)
  {
  int      ready_bits = 0;
  uint32   col;
  uint32   src_byte = 0, high_bit = 0;
  uint32   bit_offset = 0;
  uint16   match_bits = 0, mask_bits = 0;
  uint16   buff1 = 0, buff2 = 0;
  uint8    bytebuff = 0;
  unsigned char *src;
  unsigned char *dst;
  tsample_t sample;
  if ((ibuff == NULL) || (obuff == NULL))
    {
    TIFFError("reverseSample16bits","Invalid image or work buffer");
    return (1);
    }
  ready_bits = 0;
  mask_bits =  (uint16)-1 >> (16 - bps);
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
      match_bits = mask_bits << (16 - high_bit - bps); 
      if (little_endian)
        buff1 = (src[0] << 8) | src[1];
      else
        buff1 = (src[1] << 8) | src[0];
      buff1 = (buff1 & match_bits) << (high_bit);
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
  if (ready_bits > 0)
    {
    bytebuff = (buff2 >> 8);
    *dst++ = bytebuff;
    }
  return (0);
  } 

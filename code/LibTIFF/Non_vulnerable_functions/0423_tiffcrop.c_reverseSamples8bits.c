static int
reverseSamples8bits (uint16 spp, uint16 bps, uint32 width, 
                     uint8 *ibuff, uint8 *obuff)
  {
  int      ready_bits = 0;
  uint32   col;
  uint32   src_byte, src_bit;
  uint32   bit_offset = 0;
  uint8    match_bits = 0, mask_bits = 0;
  uint8    buff1 = 0, buff2 = 0;
  unsigned char *src;
  unsigned char *dst;
  tsample_t sample;
  if ((ibuff == NULL) || (obuff == NULL))
    {
    TIFFError("reverseSamples8bits","Invalid image or work buffer");
    return (1);
    }
  ready_bits = 0;
  mask_bits =  (uint8)-1 >> ( 8 - bps);
  dst = obuff;
  for (col = width; col > 0; col--)
    {
    
    bit_offset = (col - 1) * bps * spp;
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
      src = ibuff + src_byte;
      match_bits = mask_bits << (8 - src_bit - bps); 
      buff1 = ((*src) & match_bits) << (src_bit);
      if (ready_bits < 8)
        buff2 = (buff2 | (buff1 >> ready_bits));
      else  
        {
        *dst++ = buff2;
        buff2 = buff1;
        ready_bits -= 8;
        }
      ready_bits += bps;
      }
    }
  if (ready_bits > 0)
    {
    buff1 = (buff2 & ((unsigned int)255 << (8 - ready_bits)));
    *dst++ = buff1;
    }
  return (0);
  } 

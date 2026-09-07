static int 
extractContigSamplesBytes (uint8 *in, uint8 *out, uint32 cols, 
                           tsample_t sample, uint16 spp, uint16 bps, 
                           tsample_t count, uint32 start, uint32 end)
  {
  int i, bytes_per_sample, sindex;
  uint32 col, dst_rowsize, bit_offset;
  uint32 src_byte ;
  uint8 *src = in;
  uint8 *dst = out;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("extractContigSamplesBytes","Invalid input or output buffer");
    return (1);
    }
  if ((start > end) || (start > cols))
    {
    TIFFError ("extractContigSamplesBytes", 
               "Invalid start column value %d ignored", start);
    start = 0;
    }
  if ((end == 0) || (end > cols))
    {
    TIFFError ("extractContigSamplesBytes", 
               "Invalid end column value %d ignored", end);
    end = cols;
    }
  dst_rowsize = (bps * (end - start) * count) / 8;
  bytes_per_sample = (bps + 7) / 8; 
  
  if (count == spp)
    {
    src = in + (start * spp * bytes_per_sample);
    _TIFFmemcpy (dst, src, dst_rowsize);
    }
  else
    {
    for (col = start; col < end; col++)
      {
      for (sindex = sample; (sindex < spp) && (sindex < (sample + count)); sindex++)
        {
        bit_offset = col * bps * spp;
        if (sindex == 0)
          {
          src_byte = bit_offset / 8;
          
          }
        else
          {
          src_byte = (bit_offset + (sindex * bps)) / 8;
          
          }
        src = in + src_byte;
        for (i = 0; i < bytes_per_sample; i++)
            *dst++ = *src++;
        }
      }
    }
  return (0);
  } 

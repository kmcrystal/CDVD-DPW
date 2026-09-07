static int 
combineSeparateTileSamplesBytes (unsigned char *srcbuffs[], unsigned char *out,
                                 uint32 cols, uint32 rows, uint32 imagewidth,
                                 uint32 tw, uint16 spp, uint16 bps,
                                 FILE *dumpfile, int format, int level)
  {
  int i, bytes_per_sample;
  uint32 row, col, col_offset, src_rowsize, dst_rowsize, src_offset;
  unsigned char *src;
  unsigned char *dst;
  tsample_t s;
  src = srcbuffs[0];
  dst = out;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("combineSeparateTileSamplesBytes","Invalid buffer address");
    return (1);
    }
  bytes_per_sample = (bps + 7) / 8; 
  src_rowsize = ((bps * tw) + 7) / 8;
  dst_rowsize = imagewidth * bytes_per_sample * spp;
  for (row = 0; row < rows; row++)
    {
    if ((dumpfile != NULL) && (level == 2))
      {
      for (s = 0; s < spp; s++)
        {
        dump_info (dumpfile, format, "combineSeparateTileSamplesBytes","Input data, Sample %d", s);
        dump_buffer(dumpfile, format, 1, cols, row, srcbuffs[s] + (row * src_rowsize));
        }
      }
    dst = out + (row * dst_rowsize);
    src_offset = row * src_rowsize;
#ifdef DEVELMODE
    TIFFError("","Tile row %4d, Src offset %6d   Dst offset %6d", 
              row, src_offset, dst - out);
#endif
    for (col = 0; col < cols; col++)
      {
      col_offset = src_offset + (col * (bps / 8)); 
      for (s = 0; (s < spp) && (s < MAX_SAMPLES); s++)
        {
        src = srcbuffs[s] + col_offset; 
        for (i = 0; i < bytes_per_sample; i++)
          *(dst + i) = *(src + i);
        dst += bytes_per_sample;
        }   
      }
    if ((dumpfile != NULL) && (level == 2))
      {
      dump_info (dumpfile, format, "combineSeparateTileSamplesBytes","Output data, combined samples");
      dump_buffer(dumpfile, format, 1, dst_rowsize, row, out + (row * dst_rowsize));
      }
    }
  return (0);
  } 

static int 
combineSeparateSamplesBytes (unsigned char *srcbuffs[], unsigned char *out,
                             uint32 cols, uint32 rows, uint16 spp, uint16 bps,
                             FILE *dumpfile, int format, int level)
  {
  int i, bytes_per_sample;
  uint32 row, col, col_offset, src_rowsize, dst_rowsize, row_offset;
  unsigned char *src;
  unsigned char *dst;
  tsample_t s;
  src = srcbuffs[0];
  dst = out;
  if ((src == NULL) || (dst == NULL))
    {
    TIFFError("combineSeparateSamplesBytes","Invalid buffer address");
    return (1);
    }
  bytes_per_sample = (bps + 7) / 8; 
  src_rowsize = ((bps * cols) + 7) / 8;
  dst_rowsize = ((bps * spp * cols) + 7) / 8;
  for (row = 0; row < rows; row++)
    {
    if ((dumpfile != NULL) && (level == 2))
      {
      for (s = 0; s < spp; s++)
        {
        dump_info (dumpfile, format, "combineSeparateSamplesBytes","Input data, Sample %d", s);
        dump_buffer(dumpfile, format, 1, cols, row, srcbuffs[s] + (row * src_rowsize));
        }
      }
    dst = out + (row * dst_rowsize);
    row_offset = row * src_rowsize;
    for (col = 0; col < cols; col++)
      {
      col_offset = row_offset + (col * (bps / 8)); 
      for (s = 0; (s < spp) && (s < MAX_SAMPLES); s++)
        {
        src = srcbuffs[s] + col_offset; 
        for (i = 0; i < bytes_per_sample; i++)
          *(dst + i) = *(src + i);
        src += bytes_per_sample;
        dst += bytes_per_sample;
        }   
      }
    if ((dumpfile != NULL) && (level == 2))
      {
      dump_info (dumpfile, format, "combineSeparateSamplesBytes","Output data, combined samples");
      dump_buffer(dumpfile, format, 1, dst_rowsize, row, out + (row * dst_rowsize));
      }
    }
  return (0);
  } 

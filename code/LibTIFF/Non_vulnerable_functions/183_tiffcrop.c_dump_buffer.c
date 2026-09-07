  }
static int dump_buffer (FILE* dumpfile, int format, uint32 rows, uint32 width, 
                 uint32 row, unsigned char *buff)
  {
  int j, k;
  uint32 i;
  unsigned char * dump_ptr;
  if (dumpfile == NULL)
    {
    TIFFError ("", "Invalid FILE pointer for dump file");
    return (1);
    }
  for (i = 0; i < rows; i++)
    {
    dump_ptr = buff + (i * width);
    if (format == DUMP_TEXT)
      dump_info (dumpfile, format, "", 
                 "Row %4d, %d bytes at offset %d",
	         row + i + 1, width, row * width);
    for (j = 0, k = width; k >= 10; j += 10, k -= 10, dump_ptr += 10)
      dump_data (dumpfile, format, "", dump_ptr, 10);
    if (k > 0)
      dump_data (dumpfile, format, "", dump_ptr, k);
    }
  return (0);
  }

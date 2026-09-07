
static int dump_data (FILE *dumpfile, int format, char *dump_tag, unsigned char *data, uint32 count)
  {
  int j, k;
  uint32 i;
  char  dump_array[10];
  unsigned char bitset;
  if (dumpfile == NULL)
    {
    TIFFError ("", "Invalid FILE pointer for dump file");
    return (1);
    }
  if (format == DUMP_TEXT)
    {
    fprintf (dumpfile," %s  ", dump_tag);
    for (i = 0; i < count; i++)
      {
      for (j = 0, k = 7; j < 8; j++, k--)
        {
	bitset = (*(data + i)) & (((unsigned char)1 << k)) ? 1 : 0;
        sprintf(&dump_array[j], (bitset) ? "1" : "0");
        }
      dump_array[8] = '\0';
      fprintf (dumpfile," %s", dump_array);
      }
    fprintf (dumpfile,"\n");
    }
  else
    {
    if ((fwrite (data, 1, count, dumpfile)) != count)
      {
      TIFFError ("", "Unable to write binary data to dump file");
      return (1);
      }
    }
  return (0);
  }

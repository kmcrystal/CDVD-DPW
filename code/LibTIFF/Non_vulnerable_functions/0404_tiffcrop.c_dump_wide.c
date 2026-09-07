  }
static int dump_wide (FILE *dumpfile, int format, char *dump_tag, uint64 data)
  {
  int j, k;
  char  dump_array[80];
  unsigned char bitset;
  if (dumpfile == NULL)
    {
    TIFFError ("", "Invalid FILE pointer for dump file");
    return (1);
    }
  if (format == DUMP_TEXT)
    {
    fprintf (dumpfile," %s  ", dump_tag);
    for (j = 0, k = 63; k >= 0; j++, k--)
      {
      bitset = data & (((uint64)1 << k)) ? 1 : 0;
      sprintf(&dump_array[j], (bitset) ? "1" : "0");
      if ((k % 8) == 0)
          sprintf(&dump_array[++j], " ");
      }
    dump_array[71] = '\0';
    fprintf (dumpfile," %s\n", dump_array);
    }
  else
    {
    if ((fwrite (&data, 8, 1, dumpfile)) != 8)
      {
      TIFFError ("", "Unable to write binary data to dump file");
      return (1);
      }
    }
  return (0);
  }

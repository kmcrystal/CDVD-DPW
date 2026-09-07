  }
static int dump_short (FILE *dumpfile, int format, char *dump_tag, uint16 data)
  {
  int j, k;
  char  dump_array[20];
  unsigned char bitset;
  if (dumpfile == NULL)
    {
    TIFFError ("", "Invalid FILE pointer for dump file");
    return (1);
    }
  if (format == DUMP_TEXT)
    {
    fprintf (dumpfile," %s  ", dump_tag);
    for (j = 0, k = 15; k >= 0; j++, k--)
      {
      bitset = data & (((unsigned char)1 << k)) ? 1 : 0;
      sprintf(&dump_array[j], (bitset) ? "1" : "0");
      if ((k % 8) == 0)
          sprintf(&dump_array[++j], " ");
      }
    dump_array[17] = '\0';
    fprintf (dumpfile," %s\n", dump_array);
    }
  else
    {
    if ((fwrite (&data, 2, 1, dumpfile)) != 2)
      {
      TIFFError ("", "Unable to write binary data to dump file");
      return (1);
      }
    }
  return (0);
  }

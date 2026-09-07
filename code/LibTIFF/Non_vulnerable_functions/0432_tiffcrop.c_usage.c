static void
usage(void)
  {
  int i;
  fprintf(stderr, "\n%s\n", TIFFGetVersion());
  for (i = 0; usage_info[i] != NULL; i++)
    fprintf(stderr, "%s\n", usage_info[i]);
  exit(-1);
  }

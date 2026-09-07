   }
static void initDumpOptions(struct dump_opts *dump)
  {
  dump->debug  = 0;
  dump->format = DUMP_NONE;
  dump->level  = 1;
  sprintf (dump->mode, "w");
  memset (dump->infilename, '\0', PATH_MAX + 1);
  memset (dump->outfilename, '\0',PATH_MAX + 1);
  dump->infile = NULL;
  dump->outfile = NULL;
  }

static void
print_opts(png_uint_32 opts)
{
   if (opts & READ_FILE)
      printf(" --file");
   if (opts & USE_STDIO)
      printf(" --stdio");
   if (opts & STRICT)
      printf(" --strict");
   if (opts & VERBOSE)
      printf(" --verbose");
   if (opts & KEEP_TMPFILES)
      printf(" --preserve");
   if (opts & KEEP_GOING)
      printf(" --keep-going");
   if (opts & ACCUMULATE)
      printf(" --accumulate");
   if (!(opts & FAST_WRITE)) 
      printf(" --slow");
   if (opts & sRGB_16BIT)
      printf(" --sRGB-16bit");
}

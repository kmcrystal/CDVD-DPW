static void
checkbuffer(Image *image, const char *arg)
{
   if (check16(image->buffer, 95))
   {
      fflush(stdout);
      fprintf(stderr, "%s: overwrite at start of image buffer\n", arg);
      exit(1);
   }
   if (check16(image->buffer+16+image->allocsize, 95))
   {
      fflush(stdout);
      fprintf(stderr, "%s: overwrite at end of image buffer\n", arg);
      exit(1);
   }
}

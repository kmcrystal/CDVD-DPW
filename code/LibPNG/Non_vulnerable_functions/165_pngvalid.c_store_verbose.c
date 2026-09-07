static void
store_verbose(png_store *ps, png_const_structp pp, png_const_charp prefix,
   png_const_charp message)
{
   char buffer[512];
   if (prefix)
      fputs(prefix, stderr);
   (void)store_message(ps, pp, buffer, sizeof buffer, 0, message);
   fputs(buffer, stderr);
   fputc('\n', stderr);
}

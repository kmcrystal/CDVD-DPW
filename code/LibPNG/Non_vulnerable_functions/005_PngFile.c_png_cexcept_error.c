static void
png_cexcept_error(png_structp png_ptr, png_const_charp msg)
{
   if(png_ptr)
     ;
#ifdef PNG_CONSOLE_IO_SUPPORTED
   fprintf(stderr, "libpng error: %s\n", msg);
#endif
   {
      Throw msg;
   }
}

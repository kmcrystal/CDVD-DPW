static void PNGCBAPI
pngtest_warning(png_structp png_ptr, png_const_charp message)
{
   PNG_CONST char *name = "UNKNOWN (ERROR!)";
   char *test;
   test = png_get_error_ptr(png_ptr);
   if (test == NULL)
     fprintf(STDERR, "%s: libpng warning: %s\n", name, message);
   else
     fprintf(STDERR, "%s: libpng warning: %s\n", test, message);
}

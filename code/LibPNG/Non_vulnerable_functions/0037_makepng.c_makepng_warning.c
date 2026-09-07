static void PNGCBAPI
makepng_warning(png_structp png_ptr, png_const_charp message)
{
   const char **ep = png_get_error_ptr(png_ptr);
   const char *name;
   if (ep != NULL && *ep != NULL)
      name = *ep;
   else
      name = "makepng";
  fprintf(stderr, "%s: warning: %s\n", name, message);
}

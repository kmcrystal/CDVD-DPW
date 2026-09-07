
PNG_FUNCTION(void, (PNGCBAPI error), (png_structp png_ptr, const char *message),
   static PNG_NORETURN)
{
   display *d = (display*)png_get_error_ptr(png_ptr);
   fprintf(stderr, "%s(%s): libpng error: %s\n", d->file, d->test, message);
   display_exit(d);
}

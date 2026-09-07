static void PNGCBAPI
warning(png_structp png_ptr, const char *message)
{
   display *d = (display*)png_get_error_ptr(png_ptr);
   fprintf(stderr, "%s(%s): libpng warning: %s\n", d->file, d->test, message);
   ++(d->warning_count);
}

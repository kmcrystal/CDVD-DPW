#define png_error png_warning
void png_warning(png_const_structrp png_ptr, png_const_charp msg)
{
   fprintf(stderr, "validation: %s\n", msg);
}

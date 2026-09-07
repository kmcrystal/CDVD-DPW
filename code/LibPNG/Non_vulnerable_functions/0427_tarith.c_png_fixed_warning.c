#define png_fixed_error png_fixed_warning
void png_fixed_warning(png_const_structrp png_ptr, png_const_charp msg)
{
   fprintf(stderr, "overflow in: %s\n", msg);
}

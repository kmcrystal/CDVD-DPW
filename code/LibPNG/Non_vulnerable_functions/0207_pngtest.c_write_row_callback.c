void PNGCBAPI
write_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
{
   if (png_ptr == NULL || row_number > PNG_UINT_31_MAX || pass > 7)
      return;
   fprintf(stdout, "w");
}

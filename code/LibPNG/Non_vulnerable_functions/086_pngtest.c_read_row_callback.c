void PNGCBAPI
read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
{
   if (png_ptr == NULL || row_number > PNG_UINT_31_MAX)
      return;
   if (status_pass != pass)
   {
      fprintf(stdout, "\n Pass %d: ", pass);
      status_pass = pass;
      status_dots = 31;
   }
   status_dots--;
   if (status_dots == 0)
   {
      fprintf(stdout, "\n         ");
      status_dots=30;
   }
   fprintf(stdout, "r");
}

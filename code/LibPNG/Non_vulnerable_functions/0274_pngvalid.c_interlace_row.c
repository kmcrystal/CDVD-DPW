static void
interlace_row(png_bytep buffer, png_const_bytep imageRow,
   unsigned int pixel_size, png_uint_32 w, int pass)
{
   png_uint_32 xin, xout, xstep;
   



   xin = PNG_PASS_START_COL(pass);
   xstep = 1U<<PNG_PASS_COL_SHIFT(pass);
   for (xout=0; xin<w; xin+=xstep)
   {
      pixel_copy(buffer, xout, imageRow, xin, pixel_size);
      ++xout;
   }
}

static void
deinterlace_row(png_bytep buffer, png_const_bytep row,
   unsigned int pixel_size, png_uint_32 w, int pass)
{
   




   png_uint_32 xin, xout, xstep;
   xout = PNG_PASS_START_COL(pass);
   xstep = 1U<<PNG_PASS_COL_SHIFT(pass);
   for (xin=0; xout<w; xout+=xstep)
   {
      pixel_copy(buffer, xout, row, xin, pixel_size);
      ++xin;
   }
}

static void
standard_row(png_const_structp pp, png_byte std[STANDARD_ROWMAX],
   png_uint_32 id, png_uint_32 y)
{
   if (WIDTH_FROM_ID(id) == 0)
      transform_row(pp, std, COL_FROM_ID(id), DEPTH_FROM_ID(id), y);
   else
      size_row(std, WIDTH_FROM_ID(id) * bit_size(pp, COL_FROM_ID(id),
         DEPTH_FROM_ID(id)), y);
}

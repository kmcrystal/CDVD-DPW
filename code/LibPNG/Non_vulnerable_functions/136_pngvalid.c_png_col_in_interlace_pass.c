static int
png_col_in_interlace_pass(png_uint_32 x, int pass)
{
   
   int y;
   x &= 7;
   ++pass;
   for (y=0; y<8; ++y) if (adam7[y][x] == pass)
      return 1;
   return 0;
}

static int
png_row_in_interlace_pass(png_uint_32 y, int pass)
{
   
   int x;
   y &= 7;
   ++pass;
   for (x=0; x<8; ++x) if (adam7[y][x] == pass)
      return 1;
   return 0;
}

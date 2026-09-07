static png_uint_32
png_pass_start_col(int pass)
{
   int x, y;
   ++pass;
   for (x=0; x<8; ++x) for (y=0; y<8; ++y) if (adam7[y][x] == pass)
      return x;
   return 0xf;
}

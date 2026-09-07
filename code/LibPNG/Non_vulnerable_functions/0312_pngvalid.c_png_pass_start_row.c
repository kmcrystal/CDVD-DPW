static png_uint_32
png_pass_start_row(int pass)
{
   int x, y;
   ++pass;
   for (y=0; y<8; ++y) for (x=0; x<8; ++x) if (adam7[y][x] == pass)
      return y;
   return 0xf;
}

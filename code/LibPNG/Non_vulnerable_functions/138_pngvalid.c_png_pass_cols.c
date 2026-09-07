static png_uint_32
png_pass_cols(png_uint_32 width, int pass)
{
   png_uint_32 tiles = width>>3;
   png_uint_32 cols = 0;
   unsigned int x, y;
   width &= 7;
   ++pass;
   for (x=0; x<8; ++x) for (y=0; y<8; ++y) if (adam7[y][x] == pass)
   {
      cols += tiles;
      if (x < width) ++cols;
      break; 
   }
   return cols;
}

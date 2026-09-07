static int
png_pass_col_shift(int pass)
{
   int x, y, base=(-1), inc=8;
   ++pass;
   for (x=0; x<8; ++x) for (y=0; y<8; ++y) if (adam7[y][x] == pass)
   {
      if (base == (-1))
         base = x;
      else if (base == x)
         {}
      else if (inc == x-base)
         base=x;
      else if (inc == 8)
         inc = x-base, base=x;
      else if (inc != x-base)
         return 0xff; 
   }
   if (base == (-1)) return 0xfe; 
   
   switch (inc)
   {
case 1: return 0; 
case 2: return 1;
case 4: return 2;
case 8: return 3;
default: break;
   }
   
   return (inc << 8) + 0xfd;
}

static int
check16(png_const_bytep bp, int b)
{
   int i = 16;
   do
      if (*bp != b) return 1;
   while (--i);
   return 0;
}

static int
pixel_cmp(png_const_bytep pa, png_const_bytep pb, png_uint_32 bit_width)
{
#if PNG_LIBPNG_VER < 10506
   if (memcmp(pa, pb, bit_width>>3) == 0)
   {
      png_uint_32 p;
      if ((bit_width & 7) == 0) return 0;
      
      p = pa[bit_width >> 3];
      p ^= pb[bit_width >> 3];
      if (p == 0) return 0;
      


      bit_width &= 7;
      p >>= 8-bit_width;
      if (p == 0) return 0;
   }
#else
   


   if (memcmp(pa, pb, (bit_width+7)>>3) == 0)
      return 0;
#endif
   
   {
      png_uint_32 where = 0;
      while (pa[where] == pb[where]) ++where;
      return 1+where;
   }
}

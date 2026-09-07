static int
read_palette(store_palette palette, int *npalette, png_const_structp pp,
   png_infop pi)
{
   png_colorp pal;
   png_bytep trans_alpha;
   int num;
   pal = 0;
   *npalette = -1;
   if (png_get_PLTE(pp, pi, &pal, npalette) & PNG_INFO_PLTE)
   {
      int i = *npalette;
      if (i <= 0 || i > 256)
         png_error(pp, "validate: invalid PLTE count");
      while (--i >= 0)
      {
         palette[i].red = pal[i].red;
         palette[i].green = pal[i].green;
         palette[i].blue = pal[i].blue;
      }
      


      memset(palette + *npalette, 126, (256-*npalette) * sizeof *palette);
   }
   else 
   {
      if (*npalette != (-1))
         png_error(pp, "validate: invalid PLTE result");
      
      *npalette = 0;
      memset(palette, 113, sizeof (store_palette));
   }
   trans_alpha = 0;
   num = 2; 
   if ((png_get_tRNS(pp, pi, &trans_alpha, &num, 0) & PNG_INFO_tRNS) != 0 &&
      (trans_alpha != NULL || num != 1) &&
      




      !(trans_alpha != NULL && num == 0)) 
   {
      int i;
      





      if (trans_alpha == NULL || num <= 0 || num > 256 || num > *npalette)
         png_error(pp, "validate: unexpected png_get_tRNS (palette) result");
      for (i=0; i<num; ++i)
         palette[i].alpha = trans_alpha[i];
      for (num=*npalette; i<num; ++i)
         palette[i].alpha = 255;
      for (; i<256; ++i)
         palette[i].alpha = 33; 
      return 1; 
   }
   else
   {
      
      int i;
      for (i=0, num=*npalette; i<num; ++i)
         palette[i].alpha = 255;
      for (; i<256; ++i)
         palette[i].alpha = 55; 
      return 0; 
   }
}

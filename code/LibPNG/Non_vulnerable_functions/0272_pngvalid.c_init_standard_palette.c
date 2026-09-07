static void
init_standard_palette(png_store *ps, png_structp pp, png_infop pi, int npalette,
   int do_tRNS)
{
   store_palette_entry *ppal = make_standard_palette(ps, npalette, do_tRNS);
   {
      int i;
      png_color palette[256];
      
      for (i=0; i<npalette; ++i)
      {
         palette[i].red = ppal[i].red;
         palette[i].green = ppal[i].green;
         palette[i].blue = ppal[i].blue;
      }
      
      for (; i<256; ++i)
         palette[i].red = palette[i].green = palette[i].blue = 42;
      png_set_PLTE(pp, pi, palette, npalette);
   }
   if (do_tRNS)
   {
      int i, j;
      png_byte tRNS[256];
      
      for (i=j=0; i<npalette; ++i)
         if ((tRNS[i] = ppal[i].alpha) < 255)
            j = i+1;
      
      for (; i<256; ++i)
         tRNS[i] = 24;
#  ifdef PNG_WRITE_tRNS_SUPPORTED
         if (j > 0)
            png_set_tRNS(pp, pi, tRNS, j, 0);
#  endif
   }
}

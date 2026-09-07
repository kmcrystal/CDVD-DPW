static store_palette_entry *
make_standard_palette(png_store* ps, int npalette, int do_tRNS)
{
   static png_uint_32 palette_seed[2] = { 0x87654321, 9 };
   int i = 0;
   png_byte values[256][4];
   

   for (; i<8; ++i)
   {
      values[i][1] = (png_byte)((i&1) ? 255U : 0U);
      values[i][2] = (png_byte)((i&2) ? 255U : 0U);
      values[i][3] = (png_byte)((i&4) ? 255U : 0U);
   }
   
   {
      int j = 0;
      png_byte random_bytes[4];
      png_byte need[256];
      need[0] = 0; 
      memset(need+1, 1, (sizeof need)-2); 
      need[255] = 0; 
      while (i<70)
      {
         png_byte b;
         if (j==0)
         {
            make_four_random_bytes(palette_seed, random_bytes);
            j = 4;
         }
         b = random_bytes[--j];
         if (need[b])
         {
            values[i][1] = b;
            values[i][2] = b;
            values[i++][3] = b;
         }
      }
   }
   




   for (; i<256; ++i)
      make_four_random_bytes(palette_seed, values[i]);
   


   {
      store_palette_entry *palette;
      png_byte selector[4];
      make_four_random_bytes(palette_seed, selector);
      if (do_tRNS)
         for (i=0; i<256; ++i)
            values[i][0] = (png_byte)(i ^ selector[0]);
      else
         for (i=0; i<256; ++i)
            values[i][0] = 255; 
      








      palette = store_write_palette(ps, npalette);
      for (i=0; i<npalette; ++i)
      {
         palette[i].alpha = values[i ^ selector[1]][0];
         palette[i].red   = values[i ^ selector[1]][1];
         palette[i].green = values[i ^ selector[1]][2];
         palette[i].blue  = values[i ^ selector[1]][3];
      }
      return palette;
   }
}

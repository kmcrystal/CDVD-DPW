static void
standard_palette_init(standard_display *dp)
{
   store_palette_entry *palette = store_current_palette(dp->ps, &dp->npalette);
   
   if (dp->npalette > 0)
   {
      int i = dp->npalette;
      memcpy(dp->palette, palette, i * sizeof *palette);
      
      while (--i >= 0)
         if (palette[i].alpha < 255)
            break;
#     ifdef __GNUC__
         
         if (i >= 0)
            dp->is_transparent = 1;
         else
            dp->is_transparent = 0;
#     else
         dp->is_transparent = (i >= 0);
#     endif
   }
}

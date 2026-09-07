static void
standard_palette_validate(standard_display *dp, png_const_structp pp,
   png_infop pi)
{
   int npalette;
   store_palette palette;
   if (read_palette(palette, &npalette, pp, pi) != dp->is_transparent)
      png_error(pp, "validate: palette transparency changed");
   if (npalette != dp->npalette)
   {
      size_t pos = 0;
      char msg[64];
      pos = safecat(msg, sizeof msg, pos, "validate: palette size changed: ");
      pos = safecatn(msg, sizeof msg, pos, dp->npalette);
      pos = safecat(msg, sizeof msg, pos, " -> ");
      pos = safecatn(msg, sizeof msg, pos, npalette);
      png_error(pp, msg);
   }
   {
      int i = npalette; 
      while (--i >= 0)
         if (palette[i].red != dp->palette[i].red ||
            palette[i].green != dp->palette[i].green ||
            palette[i].blue != dp->palette[i].blue ||
            palette[i].alpha != dp->palette[i].alpha)
            png_error(pp, "validate: PLTE or tRNS chunk changed");
   }
}

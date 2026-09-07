static store_palette_entry *
store_write_palette(png_store *ps, int npalette)
{
   if (ps->pwrite == NULL)
      store_log(ps, NULL, "attempt to write palette without write stream", 1);
   if (ps->palette != NULL)
      png_error(ps->pwrite, "multiple store_write_palette calls");
   
   if (npalette > 0)
   {
      ps->palette = voidcast(store_palette_entry*, malloc(npalette *
         sizeof *ps->palette));
      if (ps->palette == NULL)
         png_error(ps->pwrite, "store new palette: OOM");
      ps->npalette = npalette;
   }
   return ps->palette;
}

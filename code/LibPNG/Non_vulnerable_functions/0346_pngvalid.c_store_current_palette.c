static store_palette_entry *
store_current_palette(png_store *ps, int *npalette)
{
   


   if (ps->current == NULL)
      store_log(ps, ps->pread, "no current stream for palette", 1);
   
   *npalette = ps->current->npalette;
   return ps->current->palette;
}

static void
store_freenew(png_store *ps)
{
   store_freebuffer(&ps->new);
   ps->writepos = 0;
   if (ps->palette != NULL)
   {
      free(ps->palette);
      ps->palette = NULL;
      ps->npalette = 0;
   }
}

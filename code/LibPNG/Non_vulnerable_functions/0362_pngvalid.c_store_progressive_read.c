static void
store_progressive_read(png_store *ps, png_structp pp, png_infop pi)
{
   


   if (ps->pread != pp || ps->current == NULL || ps->next == NULL)
      png_error(pp, "store state damaged (progressive)");
   do
   {
      if (ps->readpos != 0)
         png_error(pp, "store_read called during progressive read");
      png_process_data(pp, pi, ps->next->buffer, store_read_buffer_size(ps));
   }
   while (store_read_buffer_next(ps));
}

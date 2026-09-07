static void
store_read_imp(png_store *ps, png_bytep pb, png_size_t st)
{
   if (ps->current == NULL || ps->next == NULL)
      png_error(ps->pread, "store state damaged");
   while (st > 0)
   {
      size_t cbAvail = store_read_buffer_size(ps) - ps->readpos;
      if (cbAvail > 0)
      {
         if (cbAvail > st) cbAvail = st;
         memcpy(pb, ps->next->buffer + ps->readpos, cbAvail);
         st -= cbAvail;
         pb += cbAvail;
         ps->readpos += cbAvail;
      }
      else if (!store_read_buffer_next(ps))
         png_error(ps->pread, "read beyond end of file");
   }
}

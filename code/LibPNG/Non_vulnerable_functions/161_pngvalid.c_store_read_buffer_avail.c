static size_t
store_read_buffer_avail(png_store *ps)
{
   if (ps->current != NULL && ps->next != NULL)
   {
      png_store_buffer *next = &ps->current->data;
      size_t cbAvail = ps->current->datacount;
      while (next != ps->next && next != NULL)
      {
         next = next->prev;
         cbAvail += STORE_BUFFER_SIZE;
      }
      if (next != ps->next)
         png_error(ps->pread, "buffer read error");
      if (cbAvail > ps->readpos)
         return cbAvail - ps->readpos;
   }
   return 0;
}

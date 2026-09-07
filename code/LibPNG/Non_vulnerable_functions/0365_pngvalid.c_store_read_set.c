static void
store_read_set(png_store *ps, png_uint_32 id)
{
   png_store_file *pf = ps->saved;
   while (pf != NULL)
   {
      if (pf->id == id)
      {
         ps->current = pf;
         ps->next = NULL;
         store_read_buffer_next(ps);
         return;
      }
      pf = pf->next;
   }
   {
      size_t pos;
      char msg[FILE_NAME_SIZE+64];
      pos = standard_name_from_id(msg, sizeof msg, 0, id);
      pos = safecat(msg, sizeof msg, pos, ": file not found");
      png_error(ps->pread, msg);
   }
}

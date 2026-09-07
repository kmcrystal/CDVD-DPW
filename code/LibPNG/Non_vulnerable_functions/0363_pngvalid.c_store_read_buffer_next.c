static int
store_read_buffer_next(png_store *ps)
{
   png_store_buffer *pbOld = ps->next;
   png_store_buffer *pbNew = &ps->current->data;
   if (pbOld != pbNew)
   {
      while (pbNew != NULL && pbNew->prev != pbOld)
         pbNew = pbNew->prev;
      if (pbNew != NULL)
      {
         ps->next = pbNew;
         ps->readpos = 0;
         return 1;
      }
      png_error(ps->pread, "buffer lost");
   }
   return 0; 
}

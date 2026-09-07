static size_t
store_read_buffer_size(png_store *ps)
{
   
   if (ps->next != &ps->current->data)
      return STORE_BUFFER_SIZE;
   return ps->current->datacount;
}

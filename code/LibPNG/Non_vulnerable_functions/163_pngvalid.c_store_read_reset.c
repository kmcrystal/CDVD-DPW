static void
store_read_reset(png_store *ps)
{
#  ifdef PNG_READ_SUPPORTED
      if (ps->pread != NULL)
      {
         anon_context(ps);
         Try
            png_destroy_read_struct(&ps->pread, &ps->piread, NULL);
         Catch_anonymous
         {
            
         }
         ps->pread = NULL;
         ps->piread = NULL;
      }
#  endif
#  ifdef PNG_USER_MEM_SUPPORTED
      
      store_pool_delete(ps, &ps->read_memory_pool);
#  endif
   ps->current = NULL;
   ps->next = NULL;
   ps->readpos = 0;
   ps->validated = 0;
}

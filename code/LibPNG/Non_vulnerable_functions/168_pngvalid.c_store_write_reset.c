static void
store_write_reset(png_store *ps)
{
   if (ps->pwrite != NULL)
   {
      anon_context(ps);
      Try
         png_destroy_write_struct(&ps->pwrite, &ps->piwrite);
      Catch_anonymous
      {
         
      }
      ps->pwrite = NULL;
      ps->piwrite = NULL;
   }
   


#  ifdef PNG_USER_MEM_SUPPORTED
      store_pool_delete(ps, &ps->write_memory_pool);
#  endif
   store_freenew(ps);
}

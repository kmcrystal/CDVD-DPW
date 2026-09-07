static void PNGCBAPI
store_free(png_structp ppIn, png_voidp memory)
{
   png_const_structp pp = ppIn;
   store_pool *pool = voidcast(store_pool*, png_get_mem_ptr(pp));
   store_memory *this = voidcast(store_memory*, memory), **test;
   




   if (pp != pool->store->pread && pp != pool->store->pwrite)
      pp = NULL;
   


   --this;
   for (test = &pool->list; *test != this; test = &(*test)->next)
   {
      if (*test == NULL)
      {
         store_pool_error(pool->store, pp, "bad pointer to free");
         return;
      }
   }
   
   *test = this->next;
   this->next = NULL;
   store_memory_free(pp, pool, this);
}

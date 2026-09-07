static void
store_memory_free(png_const_structp pp, store_pool *pool, store_memory *memory)
{
   



   if (memory->pool != pool)
      store_pool_error(pool->store, pp, "memory corrupted (pool)");
   else if (memcmp(memory->mark, pool->mark, sizeof memory->mark) != 0)
      store_pool_error(pool->store, pp, "memory corrupted (start)");
   
   else
   {
      png_alloc_size_t cb = memory->size;
      if (cb > pool->max)
         store_pool_error(pool->store, pp, "memory corrupted (size)");
      else if (memcmp((png_bytep)(memory+1)+cb, pool->mark, sizeof pool->mark)
         != 0)
         store_pool_error(pool->store, pp, "memory corrupted (end)");
      
      else
         {
         pool->current -= cb;
         free(memory);
         }
   }
}

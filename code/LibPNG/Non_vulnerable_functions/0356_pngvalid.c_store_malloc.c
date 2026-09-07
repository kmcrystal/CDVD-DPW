static png_voidp PNGCBAPI
store_malloc(png_structp ppIn, png_alloc_size_t cb)
{
   png_const_structp pp = ppIn;
   store_pool *pool = voidcast(store_pool*, png_get_mem_ptr(pp));
   store_memory *new = voidcast(store_memory*, malloc(cb + (sizeof *new) +
      (sizeof pool->mark)));
   if (new != NULL)
   {
      if (cb > pool->max)
         pool->max = cb;
      pool->current += cb;
      if (pool->current > pool->limit)
         pool->limit = pool->current;
      pool->total += cb;
      new->size = cb;
      memcpy(new->mark, pool->mark, sizeof new->mark);
      memcpy((png_byte*)(new+1) + cb, pool->mark, sizeof pool->mark);
      new->pool = pool;
      new->next = pool->list;
      pool->list = new;
      ++new;
   }
   else
   {
      











      store_log(pool->store, pp, "out of memory", 1 );
   }
   return new;
}

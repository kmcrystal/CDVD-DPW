static void
store_pool_init(png_store *ps, store_pool *pool)
{
   memset(pool, 0, sizeof *pool);
   pool->store = ps;
   pool->list = NULL;
   pool->max = pool->current = pool->limit = pool->total = 0;
   pool->max_max = pool->max_limit = pool->max_total = 0;
   store_pool_mark(pool->mark);
}

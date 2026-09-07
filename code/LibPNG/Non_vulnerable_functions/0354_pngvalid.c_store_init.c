static void
store_init(png_store* ps)
{
   memset(ps, 0, sizeof *ps);
   init_exception_context(&ps->exception_context);
   store_pool_init(ps, &ps->read_memory_pool);
   store_pool_init(ps, &ps->write_memory_pool);
   ps->verbose = 0;
   ps->treat_warnings_as_errors = 0;
   ps->expect_error = 0;
   ps->expect_warning = 0;
   ps->saw_warning = 0;
   ps->speed = 0;
   ps->progressive = 0;
   ps->validated = 0;
   ps->nerrors = ps->nwarnings = 0;
   ps->pread = NULL;
   ps->piread = NULL;
   ps->saved = ps->current = NULL;
   ps->next = NULL;
   ps->readpos = 0;
   ps->image = NULL;
   ps->cb_image = 0;
   ps->cb_row = 0;
   ps->image_h = 0;
   ps->pwrite = NULL;
   ps->piwrite = NULL;
   ps->writepos = 0;
   ps->new.prev = NULL;
   ps->palette = NULL;
   ps->npalette = 0;
   ps->noptions = 0;
}

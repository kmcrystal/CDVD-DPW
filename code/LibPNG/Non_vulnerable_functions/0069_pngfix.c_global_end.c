static int
global_end(struct global *global)
{
   int rc;
   IDAT_list_end(&global->idat_cache);
   rc = global->status_code;
   CLEAR(*global);
   return rc;
}

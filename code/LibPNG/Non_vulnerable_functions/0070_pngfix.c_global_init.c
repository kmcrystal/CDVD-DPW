static void
global_init(struct global *global)
   
{
   CLEAR(*global);
   
   global->errors        = 0;
   global->warnings      = 0;
   global->quiet         = 0;
   global->verbose       = 0;
   global->idat_max      = 0;         
   global->optimize_zlib = 0;
   global->skip          = SKIP_NONE;
   global->status_code   = 0;
   IDAT_list_init(&global->idat_cache);
}

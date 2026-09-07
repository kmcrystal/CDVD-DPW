static png_structp
set_store_for_read(png_store *ps, png_infopp ppi, png_uint_32 id,
   PNG_CONST char *name)
{
   
   safecat(ps->test, sizeof ps->test, 0, name);
   if (ps->pread != NULL)
      png_error(ps->pread, "read store already in use");
   store_read_reset(ps);
   





#  ifdef PNG_USER_MEM_SUPPORTED
      if (!ps->speed)
         ps->pread = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, ps,
             store_error, store_warning, &ps->read_memory_pool, store_malloc,
             store_free);
      else
#  endif
   ps->pread = png_create_read_struct(PNG_LIBPNG_VER_STRING, ps, store_error,
      store_warning);
   if (ps->pread == NULL)
   {
      struct exception_context *the_exception_context = &ps->exception_context;
      store_log(ps, NULL, "png_create_read_struct returned NULL (unexpected)",
         1 );
      Throw ps;
   }
#  ifdef PNG_SET_OPTION_SUPPORTED
      {
         int opt;
         for (opt=0; opt<ps->noptions; ++opt)
            if (png_set_option(ps->pread, ps->options[opt].option,
               ps->options[opt].setting) == PNG_OPTION_INVALID)
                  png_error(ps->pread, "png option invalid");
      }
#  endif
   store_read_set(ps, id);
   if (ppi != NULL)
      *ppi = ps->piread = png_create_info_struct(ps->pread);
   return ps->pread;
}

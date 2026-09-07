static void
internal_error(png_store *ps, png_const_charp message)
{
   store_log(ps, NULL, message, 1 );
   
   {
      struct exception_context *the_exception_context = &ps->exception_context;
      Throw ps;
   }
}

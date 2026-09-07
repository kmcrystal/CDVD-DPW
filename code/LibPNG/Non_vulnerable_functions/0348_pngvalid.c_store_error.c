static void PNGCBAPI
store_error(png_structp ppIn, png_const_charp message) 
{
   png_const_structp pp = ppIn;
   png_store *ps = voidcast(png_store*, png_get_error_ptr(pp));
   if (!ps->expect_error)
      store_log(ps, pp, message, 1 );
   
   {
      struct exception_context *the_exception_context = &ps->exception_context;
      Throw ps;
   }
}

static void PNGCBAPI
store_warning(png_structp ppIn, png_const_charp message)
{
   png_const_structp pp = ppIn;
   png_store *ps = voidcast(png_store*, png_get_error_ptr(pp));
   if (!ps->expect_warning)
      store_log(ps, pp, message, 0 );
   else
      ps->saw_warning = 1;
}

static void PNGCBAPI
store_read(png_structp ppIn, png_bytep pb, png_size_t st)
{
   png_const_structp pp = ppIn;
   png_store *ps = voidcast(png_store*, png_get_io_ptr(pp));
   if (ps == NULL || ps->pread != pp)
      png_error(pp, "bad store read call");
   store_read_imp(ps, pb, st);
}

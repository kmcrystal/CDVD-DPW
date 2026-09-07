static void PNGCBAPI
modifier_read(png_structp ppIn, png_bytep pb, png_size_t st)
{
   png_const_structp pp = ppIn;
   png_modifier *pm = voidcast(png_modifier*, png_get_io_ptr(pp));
   if (pm == NULL || pm->this.pread != pp)
      png_error(pp, "bad modifier_read call");
   modifier_read_imp(pm, pb, st);
}

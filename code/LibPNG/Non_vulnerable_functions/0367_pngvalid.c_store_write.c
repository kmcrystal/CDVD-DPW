static void PNGCBAPI
store_write(png_structp ppIn, png_bytep pb, png_size_t st)
{
   png_const_structp pp = ppIn;
   png_store *ps = voidcast(png_store*, png_get_io_ptr(pp));
   if (ps->pwrite != pp)
      png_error(pp, "store state damaged");
   while (st > 0)
   {
      size_t cb;
      if (ps->writepos >= STORE_BUFFER_SIZE)
         store_storenew(ps);
      cb = st;
      if (cb > STORE_BUFFER_SIZE - ps->writepos)
         cb = STORE_BUFFER_SIZE - ps->writepos;
      memcpy(ps->new.buffer + ps->writepos, pb, cb);
      pb += cb;
      st -= cb;
      ps->writepos += cb;
   }
}

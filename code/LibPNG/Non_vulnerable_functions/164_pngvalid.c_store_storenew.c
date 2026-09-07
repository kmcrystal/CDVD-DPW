static void
store_storenew(png_store *ps)
{
   png_store_buffer *pb;
   if (ps->writepos != STORE_BUFFER_SIZE)
      png_error(ps->pwrite, "invalid store call");
   pb = voidcast(png_store_buffer*, malloc(sizeof *pb));
   if (pb == NULL)
      png_error(ps->pwrite, "store new: OOM");
   *pb = ps->new;
   ps->new.prev = pb;
   ps->writepos = 0;
}

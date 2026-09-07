static void
store_delete(png_store *ps)
{
   store_write_reset(ps);
   store_read_reset(ps);
   store_freefile(&ps->saved);
   store_image_free(ps, NULL);
}

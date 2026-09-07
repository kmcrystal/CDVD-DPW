static void
check_error(display *d, png_uint_32 flags, const char *message)
{
   while (flags)
   {
      png_uint_32 flag = flags & -(png_int_32)flags;
      int i = find_by_flag(flag);
      fprintf(stderr, "%s(%s): chunk %s: %s\n", d->file, d->test,
         chunk_info[i].name, message);
      ++(d->error_count);
      flags &= ~flag;
   }
}

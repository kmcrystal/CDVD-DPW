static void
display_destroy(struct display *dp)
{
    
#  ifdef PNG_WRITE_SUPPORTED
      buffer_destroy(&dp->written_file);
#  endif
   buffer_destroy(&dp->original_file);
}

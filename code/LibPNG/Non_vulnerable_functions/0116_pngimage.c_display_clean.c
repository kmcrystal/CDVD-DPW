static void
display_clean(struct display *dp)
{
#  ifdef PNG_WRITE_SUPPORTED
      display_clean_write(dp);
#  endif
   display_clean_read(dp);
   dp->original_rowbytes = 0;
   dp->original_rows = NULL;
   dp->chunks = 0;
   png_destroy_read_struct(&dp->original_pp, &dp->original_ip, NULL);
   
   dp->results = 0; 
}

static void
read_png(struct display *dp, struct buffer *bp, const char *operation,
   int transforms)
{
   png_structp pp;
   png_infop   ip;
   


   display_clean_read(dp);
   if (operation != NULL) 
   {
      dp->operation = operation;
      dp->transforms = transforms;
   }
   dp->read_pp = pp = png_create_read_struct(PNG_LIBPNG_VER_STRING, dp,
      display_error, display_warning);
   if (pp == NULL)
      display_log(dp, LIBPNG_ERROR, "failed to create read struct");
   


   dp->read_ip = ip = png_create_info_struct(pp);
   if (ip == NULL)
      display_log(dp, LIBPNG_ERROR, "failed to create info struct");
#  ifdef PNG_SET_USER_LIMITS_SUPPORTED
      
      png_set_user_limits(pp, 0x7fffffff, 0x7fffffff);
#  endif
   
   buffer_start_read(bp);
   png_set_read_fn(pp, bp, read_function);
   png_read_png(pp, ip, transforms, NULL);
#if 0 
   {
      png_bytep pr = png_get_rows(pp, ip)[0];
      size_t rb = png_get_rowbytes(pp, ip);
      size_t cb;
      char c = ' ';
      fprintf(stderr, "%.4x %2d (%3lu bytes):", transforms, png_get_bit_depth(pp,ip), (unsigned long)rb);
      for (cb=0; cb<rb; ++cb)
         fputc(c, stderr), fprintf(stderr, "%.2x", pr[cb]), c='.';
      fputc('\n', stderr);
   }
#endif
}

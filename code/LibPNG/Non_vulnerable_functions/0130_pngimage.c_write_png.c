static void
write_png(struct display *dp, png_infop ip, int transforms)
{
   display_clean_write(dp); 
   buffer_start_write(&dp->written_file);
   dp->operation = "write";
   dp->transforms = transforms;
   dp->write_pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, dp,
      display_error, display_warning);
   if (dp->write_pp == NULL)
      display_log(dp, APP_ERROR, "failed to create write png_struct");
   png_set_write_fn(dp->write_pp, &dp->written_file, write_function,
      NULL);
#  ifdef PNG_SET_USER_LIMITS_SUPPORTED
      
      png_set_user_limits(dp->write_pp, 0x7fffffff, 0x7fffffff);
#  endif
   


   if (transforms & (PNG_TRANSFORM_PACKING|
                     PNG_TRANSFORM_STRIP_FILLER|
                     PNG_TRANSFORM_STRIP_FILLER_BEFORE))
   {
      int ct = dp->color_type;
      if (transforms & (PNG_TRANSFORM_STRIP_FILLER|
                        PNG_TRANSFORM_STRIP_FILLER_BEFORE))
         ct &= ~PNG_COLOR_MASK_ALPHA;
      png_set_IHDR(dp->write_pp, ip, dp->width, dp->height, dp->bit_depth, ct,
         dp->interlace_method, dp->compression_method, dp->filter_method);
   }
   png_write_png(dp->write_pp, ip, transforms, NULL);
   


   display_clean_write(dp);
}

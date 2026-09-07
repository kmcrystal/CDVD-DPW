static png_bytep
extract(FILE *fp, png_uint_32 *proflen)
{
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
   png_infop info_ptr = NULL;
   png_bytep result = NULL;
   
   *proflen = 0;
   if (png_ptr == NULL)
   {
      fprintf(stderr, "iccfrompng: version library mismatch?\n");
      return 0;
   }
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return 0;
   }
   png_init_io(png_ptr, fp);
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
      png_error(png_ptr, "OOM allocating info structure");
   png_read_info(png_ptr, info_ptr);
   {
      png_charp name;
      int compression_type;
      png_bytep profile;
      if (png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile,
         proflen) & PNG_INFO_iCCP)
      {
         result = malloc(*proflen);
         if (result != NULL)
            memcpy(result, profile, *proflen);
         else
            png_error(png_ptr, "OOM allocating profile buffer");
      }
      else
	result = no_profile;
   }
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   return result;
}

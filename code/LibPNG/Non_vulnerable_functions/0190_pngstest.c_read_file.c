static int
read_file(Image *image, png_uint_32 format, png_const_colorp background)
{
   memset(&image->image, 0, sizeof image->image);
   image->image.version = PNG_IMAGE_VERSION;
   if (image->input_memory != NULL)
   {
      if (!png_image_begin_read_from_memory(&image->image, image->input_memory,
         image->input_memory_size))
         return logerror(image, "memory init: ", image->file_name, "");
   }
#  ifdef PNG_STDIO_SUPPORTED
      else if (image->input_file != NULL)
      {
         if (!png_image_begin_read_from_stdio(&image->image, image->input_file))
            return logerror(image, "stdio init: ", image->file_name, "");
      }
      else
      {
         if (!png_image_begin_read_from_file(&image->image, image->file_name))
            return logerror(image, "file init: ", image->file_name, "");
      }
#  else
      else
      {
         return logerror(image, "unsupported file/stdio init: ",
            image->file_name, "");
      }
#  endif
   
   if (image->opts & sRGB_16BIT)
      image->image.flags |= PNG_IMAGE_FLAG_16BIT_sRGB;
   


   {
      int result;
      png_uint_32 image_format;
      
      image_format = image->image.format;
      if (image->opts & VERBOSE)
      {
         printf("%s %lu x %lu %s -> %s", image->file_name,
            (unsigned long)image->image.width,
            (unsigned long)image->image.height,
            format_names[image_format & FORMAT_MASK],
            (format & FORMAT_NO_CHANGE) != 0 || image->image.format == format
            ? "no change" : format_names[format & FORMAT_MASK]);
         if (background != NULL)
            printf(" background(%d,%d,%d)\n", background->red,
               background->green, background->blue);
         else
            printf("\n");
         fflush(stdout);
      }
      



      if ((format & FORMAT_NO_CHANGE) != 0)
      {
         if ((format & PNG_FORMAT_FLAG_COLORMAP) != 0 &&
            (image_format & PNG_FORMAT_FLAG_COLORMAP) != 0)
            format = (image_format & ~BASE_FORMATS) | (format & BASE_FORMATS);
         else
            format = image_format;
      }
      image->image.format = format;
      image->stride = PNG_IMAGE_ROW_STRIDE(image->image) + image->stride_extra;
      allocbuffer(image);
      result = png_image_finish_read(&image->image, background,
         image->buffer+16, (png_int_32)image->stride, image->colormap);
      checkbuffer(image, image->file_name);
      if (result)
         return checkopaque(image);
      else
         return logerror(image, image->file_name, ": image read failed", "");
   }
}

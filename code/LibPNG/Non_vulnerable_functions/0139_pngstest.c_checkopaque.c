static int
checkopaque(Image *image)
{
   if (image->image.opaque != NULL)
   {
      png_image_free(&image->image);
      return logerror(image, image->file_name, ": opaque not NULL", "");
   }
   else if (image->image.warning_or_error != 0 && (image->opts & STRICT) != 0)
      return logerror(image, image->file_name, " --strict", "");
   else
      return 1;
}

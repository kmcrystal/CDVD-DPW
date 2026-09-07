static int
logerror(Image *image, const char *a1, const char *a2, const char *a3)
{
   fflush(stdout);
   if (image->image.warning_or_error)
      fprintf(stderr, "%s%s%s: %s\n", a1, a2, a3, image->image.message);
   else
      fprintf(stderr, "%s%s%s\n", a1, a2, a3);
   if (image->image.opaque != NULL)
   {
      fprintf(stderr, "%s: image opaque pointer non-NULL on error\n",
         image->file_name);
      png_image_free(&image->image);
   }
   return 0;
}

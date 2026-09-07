static int
logpixel(const Transform *transform, png_uint_32 x, png_uint_32 y,
   const Pixel *in, const Pixel *calc, const Pixel *out, const char *reason)
{
   const png_uint_32 in_format = transform->in_image->image.format;
   const png_uint_32 out_format = transform->out_image->image.format;
   png_uint_32 back_format = out_format & ~PNG_FORMAT_FLAG_ALPHA;
   const char *via_linear = "";
   char pixel_in[64], pixel_calc[64], pixel_out[64], pixel_loc[64];
   char background_info[100];
   print_pixel(pixel_in, in, in_format);
   print_pixel(pixel_calc, calc, out_format);
   print_pixel(pixel_out, out, out_format);
   if (transform->is_palette)
      sprintf(pixel_loc, "palette: %lu", (unsigned long)y);
   else
      sprintf(pixel_loc, "%lu,%lu", (unsigned long)x, (unsigned long)y);
   if (transform->from_linear != NULL)
   {
      via_linear = " (via linear)";
      



      back_format |= PNG_FORMAT_FLAG_LINEAR;
   }
   if (transform->background != NULL)
   {
      Pixel back;
      char pixel_back[64];
      back.r = transform->background->ir;
      back.g = transform->background->ig;
      back.b = transform->background->ib;
      back.a = -1; 
      print_pixel(pixel_back, &back, back_format);
      sprintf(background_info, " on background %s", pixel_back);
   }
   else
      background_info[0] = 0;
   if (transform->in_image->file_name != transform->out_image->file_name)
   {
      char error_buffer[512];
      sprintf(error_buffer,
         "(%s) %s error%s:\n %s%s ->\n       %s\n  not: %s.\n"
         "Use --preserve and examine: ", pixel_loc, reason, via_linear,
         pixel_in, background_info, pixel_out, pixel_calc);
      return logerror(transform->in_image, transform->in_image->file_name,
         error_buffer, transform->out_image->file_name);
   }
   else
   {
      char error_buffer[512];
      sprintf(error_buffer,
         "(%s) %s error%s:\n %s%s ->\n       %s\n  not: %s.\n"
         " The error happened when reading the original file with this format.",
         pixel_loc, reason, via_linear, pixel_in, background_info, pixel_out,
         pixel_calc);
      return logerror(transform->in_image, transform->in_image->file_name,
         error_buffer, "");
   }
}

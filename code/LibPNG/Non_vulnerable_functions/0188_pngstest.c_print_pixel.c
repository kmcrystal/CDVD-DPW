static void
print_pixel(char string[64], const Pixel *pixel, png_uint_32 format)
{
   switch (format & (PNG_FORMAT_FLAG_ALPHA|PNG_FORMAT_FLAG_COLOR))
   {
      case 0:
         sprintf(string, "%s(%d)", format_names[format], pixel->g);
         break;
      case PNG_FORMAT_FLAG_ALPHA:
         sprintf(string, "%s(%d,%d)", format_names[format], pixel->g,
            pixel->a);
         break;
      case PNG_FORMAT_FLAG_COLOR:
         sprintf(string, "%s(%d,%d,%d)", format_names[format],
            pixel->r, pixel->g, pixel->b);
         break;
      case PNG_FORMAT_FLAG_COLOR|PNG_FORMAT_FLAG_ALPHA:
         sprintf(string, "%s(%d,%d,%d,%d)", format_names[format],
            pixel->r, pixel->g, pixel->b, pixel->a);
         break;
      default:
         sprintf(string, "invalid-format");
         break;
   }
}

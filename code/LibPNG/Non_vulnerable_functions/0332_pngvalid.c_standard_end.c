static void PNGCBAPI
standard_end(png_structp ppIn, png_infop pi)
{
   png_const_structp pp = ppIn;
   standard_display *dp = voidcast(standard_display*,
      png_get_progressive_ptr(pp));
   UNUSED(pi)
   


   standard_text_validate(dp, pp, pi,
      PNG_LIBPNG_VER >= 10518);
   standard_image_validate(dp, pp, 0, -1);
}

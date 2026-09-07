static void PNGCBAPI
standard_info(png_structp pp, png_infop pi)
{
   standard_display *dp = voidcast(standard_display*,
      png_get_progressive_ptr(pp));
   


   standard_info_imp(dp, pp, pi, 1 );
}

static void PNGCBAPI
transform_info(png_structp pp, png_infop pi)
{
   transform_info_imp(voidcast(transform_display*, png_get_progressive_ptr(pp)),
      pp, pi);
}

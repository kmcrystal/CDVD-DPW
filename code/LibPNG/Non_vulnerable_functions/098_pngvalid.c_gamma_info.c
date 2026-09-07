static void PNGCBAPI
gamma_info(png_structp pp, png_infop pi)
{
   gamma_info_imp(voidcast(gamma_display*, png_get_progressive_ptr(pp)), pp,
      pi);
}

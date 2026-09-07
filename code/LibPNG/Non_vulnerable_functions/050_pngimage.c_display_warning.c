static void PNGCBAPI
display_warning(png_structp pp, png_const_charp warning)
{
   display_log(get_dp(pp), LIBPNG_WARNING, "%s", warning);
}

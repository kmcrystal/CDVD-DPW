static void PNGCBAPI
display_error(png_structp pp, png_const_charp error)
{
   struct display *dp = get_dp(pp);
   display_log(dp, LIBPNG_ERROR, "%s", error);
}

static void
standard_image_validate(standard_display *dp, png_const_structp pp, int iImage,
    int iDisplay)
{
   png_uint_32 y;
   if (iImage >= 0)
      store_image_check(dp->ps, pp, iImage);
   if (iDisplay >= 0)
      store_image_check(dp->ps, pp, iDisplay);
   for (y=0; y<dp->h; ++y)
      standard_row_validate(dp, pp, iImage, iDisplay, y);
   
   dp->ps->validated = 1;
}

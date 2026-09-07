static void PNGCBAPI
transform_end(png_structp ppIn, png_infop pi)
{
   png_const_structp pp = ppIn;
   transform_display *dp = voidcast(transform_display*,
      png_get_progressive_ptr(pp));
   if (!dp->this.speed)
      transform_image_validate(dp, pp, pi);
   else
      dp->this.ps->validated = 1;
}

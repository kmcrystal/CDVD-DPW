static void PNGCBAPI
gamma_end(png_structp ppIn, png_infop pi)
{
   png_const_structp pp = ppIn;
   gamma_display *dp = voidcast(gamma_display*, png_get_progressive_ptr(pp));
   if (!dp->this.speed)
      gamma_image_validate(dp, pp, pi);
   else
      dp->this.ps->validated = 1;
}

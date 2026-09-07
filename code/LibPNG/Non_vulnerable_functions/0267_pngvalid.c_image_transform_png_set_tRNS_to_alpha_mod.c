static void
image_transform_png_set_tRNS_to_alpha_mod(PNG_CONST image_transform *this,
   image_pixel *that, png_const_structp pp,
   PNG_CONST transform_display *display)
{
   
   if (that->colour_type == PNG_COLOR_TYPE_PALETTE)
      image_pixel_convert_PLTE(that);
   


   if (that->have_tRNS)
      image_pixel_add_alpha(that, &display->this);
   
   else
   {
      if (that->bit_depth < 8)
         that->bit_depth =8;
      if (that->sample_depth < 8)
         that->sample_depth = 8;
   }
   this->next->mod(this->next, that, pp, display);
}

static void
image_transform_mod_end(PNG_CONST image_transform *this, image_pixel *that,
    png_const_structp pp, PNG_CONST transform_display *display)
{
   PNG_CONST unsigned int scale = (1U<<that->sample_depth)-1;
   UNUSED(this)
   UNUSED(pp)
   UNUSED(display)
   






   that->red = sample_scale(that->redf, scale);
   









   that->rede += 1./(2*((1U<<that->red_sBIT)-1));
   if (that->colour_type & PNG_COLOR_MASK_COLOR)
   {
      that->green = sample_scale(that->greenf, scale);
      that->blue = sample_scale(that->bluef, scale);
      that->greene += 1./(2*((1U<<that->green_sBIT)-1));
      that->bluee += 1./(2*((1U<<that->blue_sBIT)-1));
   }
   else
   {
      that->blue = that->green = that->red;
      that->bluef = that->greenf = that->redf;
      that->bluee = that->greene = that->rede;
   }
   if ((that->colour_type & PNG_COLOR_MASK_ALPHA) ||
      that->colour_type == PNG_COLOR_TYPE_PALETTE)
   {
      that->alpha = sample_scale(that->alphaf, scale);
      that->alphae += 1./(2*((1U<<that->alpha_sBIT)-1));
   }
   else
   {
      that->alpha = scale; 
      that->alpha = 1;     
      that->alphae = 0;    
   }
}

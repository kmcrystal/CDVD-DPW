static void
transform_set_encoding(transform_display *this)
{
   


   png_modifier *pm = this->pm;
   modifier_set_encoding(pm);
   if (modifier_color_encoding_is_set(pm))
   {
      if (modifier_color_encoding_is_sRGB(pm))
         srgb_modification_init(&this->srgb_mod, pm, PNG_sRGB_INTENT_ABSOLUTE);
      else
      {
         
         gama_modification_init(&this->gama_mod, pm, pm->current_gamma);
         if (pm->current_encoding != 0)
            chrm_modification_init(&this->chrm_mod, pm, pm->current_encoding);
      }
   }
}

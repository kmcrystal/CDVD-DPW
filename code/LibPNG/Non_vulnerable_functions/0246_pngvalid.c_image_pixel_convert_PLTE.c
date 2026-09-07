static void
image_pixel_convert_PLTE(image_pixel *this)
{
   if (this->colour_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (this->have_tRNS)
      {
         this->colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
         this->have_tRNS = 0;
      }
      else
         this->colour_type = PNG_COLOR_TYPE_RGB;
      


      this->bit_depth = 8;
   }
}

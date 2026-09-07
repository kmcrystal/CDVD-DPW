static void
image_transform_png_set_rgb_to_gray_ini(PNG_CONST image_transform *this,
    transform_display *that)
{
   png_modifier *pm = that->pm;
   PNG_CONST color_encoding *e = pm->current_encoding;
   UNUSED(this)
   
   pm->test_uses_encoding = 1;
   


   if (e != 0)
   {
      


      PNG_CONST double whiteY = e->red.Y + e->green.Y + e->blue.Y;
      data.red_coefficient = e->red.Y;
      data.green_coefficient = e->green.Y;
      data.blue_coefficient = e->blue.Y;
      if (whiteY != 1)
      {
         data.red_coefficient /= whiteY;
         data.green_coefficient /= whiteY;
         data.blue_coefficient /= whiteY;
      }
   }
   else
   {
      
      data.red_coefficient = 6968 / 32768.;
      data.green_coefficient = 23434 / 32768.;
      data.blue_coefficient = 2366 / 32768.;
   }
   data.gamma = pm->current_gamma;
   
   if (data.gamma == 0)
      data.gamma = 1;
   



   if (pm->test_exhaustive)
   {
      



      data.coefficients_overridden = !data.coefficients_overridden;
      pm->repeat = data.coefficients_overridden != 0;
   }
   else
      data.coefficients_overridden = random_choice();
   if (data.coefficients_overridden)
   {
      


      png_uint_32 ru;
      double total;
      RANDOMIZE(ru);
      data.green_coefficient = total = (ru & 0xffff) / 65535.;
      ru >>= 16;
      data.red_coefficient = (1 - total) * (ru & 0xffff) / 65535.;
      total += data.red_coefficient;
      data.blue_coefficient = 1 - total;
#     ifdef PNG_FLOATING_POINT_SUPPORTED
         data.red_to_set = data.red_coefficient;
         data.green_to_set = data.green_coefficient;
#     else
         data.red_to_set = fix(data.red_coefficient);
         data.green_to_set = fix(data.green_coefficient);
#     endif
      
      pm->encoding_ignored = 1;
   }
   else
   {
      data.red_to_set = -1;
      data.green_to_set = -1;
   }
   


   if (data.gamma != 1) 
   {
      if (that->this.bit_depth == 16 || pm->assume_16_bit_calculations)
      {
         








         that->pm->limit +=
#        if PNG_MAX_GAMMA_8 < 14
               pow((that->this.bit_depth == 16 ?
                  8. : 6. + (1<<(15-PNG_MAX_GAMMA_8)))/65535, data.gamma);
#        else
               pow((that->this.bit_depth == 16 ?
                  8. : 8. + (1<<(15-PNG_MAX_GAMMA_8)))/65535, data.gamma);
#        endif
      }
      else
      {
         












         that->pm->limit +=
#        if DIGITIZE
             pow(1.1 /255, data.gamma);
#        else
             pow(1.0 /255, data.gamma);
#        endif
      }
   }
   else
   {
      


      if (that->this.bit_depth != 16 && !pm->assume_16_bit_calculations)
         that->pm->limit += 4E-3;
   }
}

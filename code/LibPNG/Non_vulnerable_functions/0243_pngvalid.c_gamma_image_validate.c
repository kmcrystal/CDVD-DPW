static void
gamma_image_validate(gamma_display *dp, png_const_structp pp,
   png_infop pi)
{
   
   PNG_CONST png_store* PNG_CONST ps = dp->this.ps;
   PNG_CONST png_byte in_ct = dp->this.colour_type;
   PNG_CONST png_byte in_bd = dp->this.bit_depth;
   PNG_CONST png_uint_32 w = dp->this.w;
   PNG_CONST png_uint_32 h = dp->this.h;
   PNG_CONST size_t cbRow = dp->this.cbRow;
   PNG_CONST png_byte out_ct = png_get_color_type(pp, pi);
   PNG_CONST png_byte out_bd = png_get_bit_depth(pp, pi);
   






































   PNG_CONST unsigned int samples_per_pixel = (out_ct & 2U) ? 3U : 1U;
   int processing;
   png_uint_32 y;
   PNG_CONST store_palette_entry *in_palette = dp->this.palette;
   PNG_CONST int in_is_transparent = dp->this.is_transparent;
   int out_npalette = -1;
   int out_is_transparent = 0; 
   store_palette out_palette;
   validate_info vi;
   
   store_image_check(dp->this.ps, pp, 0);
   


   init_validate_info(&vi, dp, pp, in_ct==3?8:in_bd, out_ct==3?8:out_bd);
   processing = (vi.gamma_correction > 0 && !dp->threshold_test)
      || in_bd != out_bd || in_ct != out_ct || vi.do_background;
   






   if (in_ct == 3 && out_ct == 3)
      processing = 0;
   if (processing && out_ct == 3)
      out_is_transparent = read_palette(out_palette, &out_npalette, pp, pi);
   for (y=0; y<h; ++y)
   {
      png_const_bytep pRow = store_image_row(ps, pp, 0, y);
      png_byte std[STANDARD_ROWMAX];
      transform_row(pp, std, in_ct, in_bd, y);
      if (processing)
      {
         unsigned int x;
         for (x=0; x<w; ++x)
         {
            double alpha = 1; 
            
            PNG_CONST unsigned int in_index =
               in_ct == 3 ? sample(std, 3, in_bd, x, 0) : 256;
            PNG_CONST unsigned int out_index =
               out_ct == 3 ? sample(std, 3, out_bd, x, 0) : 256;
            


            if ((in_ct & PNG_COLOR_MASK_ALPHA) != 0 || (in_ct == 3 &&
               in_is_transparent))
            {
               PNG_CONST unsigned int input_alpha = in_ct == 3 ?
                  dp->this.palette[in_index].alpha :
                  sample(std, in_ct, in_bd, x, samples_per_pixel);
               unsigned int output_alpha = 65536 ;
               if (out_ct == 3)
               {
                  if (out_is_transparent)
                     output_alpha = out_palette[out_index].alpha;
               }
               else if ((out_ct & PNG_COLOR_MASK_ALPHA) != 0)
                  output_alpha = sample(pRow, out_ct, out_bd, x,
                     samples_per_pixel);
               if (output_alpha != 65536)
                  alpha = gamma_component_validate("alpha", &vi, input_alpha,
                     output_alpha, -1, 0);
               else 
               {
                  


                  alpha = input_alpha >> vi.isbit_shift;
                  alpha /= vi.sbit_max;
               }
            }
            
            if ((in_ct & PNG_COLOR_MASK_COLOR) == 0) 
               (void)gamma_component_validate("gray", &vi,
                  sample(std, in_ct, in_bd, x, 0),
                  sample(pRow, out_ct, out_bd, x, 0), alpha,
                  vi.background_red);
            else 
            {
               (void)gamma_component_validate("red", &vi,
                  in_ct == 3 ? in_palette[in_index].red :
                     sample(std, in_ct, in_bd, x, 0),
                  out_ct == 3 ? out_palette[out_index].red :
                     sample(pRow, out_ct, out_bd, x, 0),
                  alpha, vi.background_red);
               (void)gamma_component_validate("green", &vi,
                  in_ct == 3 ? in_palette[in_index].green :
                     sample(std, in_ct, in_bd, x, 1),
                  out_ct == 3 ? out_palette[out_index].green :
                     sample(pRow, out_ct, out_bd, x, 1),
                  alpha, vi.background_green);
               (void)gamma_component_validate("blue", &vi,
                  in_ct == 3 ? in_palette[in_index].blue :
                     sample(std, in_ct, in_bd, x, 2),
                  out_ct == 3 ? out_palette[out_index].blue :
                     sample(pRow, out_ct, out_bd, x, 2),
                  alpha, vi.background_blue);
            }
         }
      }
      else if (memcmp(std, pRow, cbRow) != 0)
      {
         char msg[64];
         
         sprintf(msg, "gamma: below threshold row %lu changed",
            (unsigned long)y);
         png_error(pp, msg);
      }
   } 
   dp->this.ps->validated = 1;
}

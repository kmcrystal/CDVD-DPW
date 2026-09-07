static void
gamma_info_imp(gamma_display *dp, png_structp pp, png_infop pi)
{
   
   standard_info_part1(&dp->this, pp, pi);
   




   if (dp->scale16)
#     ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
         png_set_scale_16(pp);
#     else
         
#        ifdef PNG_READ_16_TO_8_SUPPORTED
            png_set_strip_16(pp);
#        else
            png_error(pp, "scale16 (16 to 8 bit conversion) not supported");
#        endif
#     endif
   if (dp->expand16)
#     ifdef PNG_READ_EXPAND_16_SUPPORTED
         png_set_expand_16(pp);
#     else
         png_error(pp, "expand16 (8 to 16 bit conversion) not supported");
#     endif
   if (dp->do_background >= ALPHA_MODE_OFFSET)
   {
#     ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      {
         
         int mode = dp->do_background - ALPHA_MODE_OFFSET;
         



         PNG_CONST double sg = dp->screen_gamma;
#        ifndef PNG_FLOATING_POINT_SUPPORTED
            PNG_CONST png_fixed_point g = fix(sg);
#        endif
#        ifdef PNG_FLOATING_POINT_SUPPORTED
            png_set_alpha_mode(pp, mode, sg);
#        else
            png_set_alpha_mode_fixed(pp, mode, g);
#        endif
         



         if (mode == PNG_ALPHA_STANDARD && sg != 1)
         {
#           ifdef PNG_FLOATING_POINT_SUPPORTED
               png_set_gamma(pp, sg, dp->file_gamma);
#           else
               png_fixed_point f = fix(dp->file_gamma);
               png_set_gamma_fixed(pp, g, f);
#           endif
         }
      }
#     else
         png_error(pp, "alpha mode handling not supported");
#     endif
   }
   else
   {
      
#     ifdef PNG_FLOATING_POINT_SUPPORTED
         png_set_gamma(pp, dp->screen_gamma, dp->file_gamma);
#     else
      {
         png_fixed_point s = fix(dp->screen_gamma);
         png_fixed_point f = fix(dp->file_gamma);
         png_set_gamma_fixed(pp, s, f);
      }
#     endif
      if (dp->do_background)
      {
#     ifdef PNG_READ_BACKGROUND_SUPPORTED
         

         PNG_CONST double bg = dp->background_gamma;
#        ifndef PNG_FLOATING_POINT_SUPPORTED
            PNG_CONST png_fixed_point g = fix(bg);
#        endif
#        ifdef PNG_FLOATING_POINT_SUPPORTED
            png_set_background(pp, &dp->background_color, dp->do_background,
               0, bg);
#        else
            png_set_background_fixed(pp, &dp->background_color,
               dp->do_background, 0, g);
#        endif
#     else
         png_error(pp, "png_set_background not supported");
#     endif
      }
   }
   {
      int i = dp->this.use_update_info;
      
      do
         png_read_update_info(pp, pi);
      while (--i > 0);
   }
   
   standard_info_part2(&dp->this, pp, pi, 1 );
}

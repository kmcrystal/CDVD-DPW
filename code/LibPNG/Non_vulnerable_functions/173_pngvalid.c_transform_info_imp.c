static void
transform_info_imp(transform_display *dp, png_structp pp, png_infop pi)
{
   
   standard_info_part1(&dp->this, pp, pi);
   
   dp->transform_list->set(dp->transform_list, dp, pp, pi);
   
   {
      int i = dp->this.use_update_info;
      
      do
         png_read_update_info(pp, pi);
      while (--i > 0);
   }
   
   standard_info_part2(&dp->this, pp, pi, 1);
   
   dp->output_colour_type = png_get_color_type(pp, pi);
   dp->output_bit_depth = png_get_bit_depth(pp, pi);
   



   switch (dp->output_colour_type)
   {
   case PNG_COLOR_TYPE_PALETTE:
      if (dp->output_bit_depth > 8) goto error;
      
   case PNG_COLOR_TYPE_GRAY:
      if (dp->output_bit_depth == 1 || dp->output_bit_depth == 2 ||
         dp->output_bit_depth == 4)
         break;
      
   default:
      if (dp->output_bit_depth == 8 || dp->output_bit_depth == 16)
         break;
      
   error:
      {
         char message[128];
         size_t pos;
         pos = safecat(message, sizeof message, 0,
            "invalid final bit depth: colour type(");
         pos = safecatn(message, sizeof message, pos, dp->output_colour_type);
         pos = safecat(message, sizeof message, pos, ") with bit depth: ");
         pos = safecatn(message, sizeof message, pos, dp->output_bit_depth);
         png_error(pp, message);
      }
   }
   


   {
      image_pixel test_pixel;
      memset(&test_pixel, 0, sizeof test_pixel);
      test_pixel.colour_type = dp->this.colour_type; 
      test_pixel.bit_depth = dp->this.bit_depth;
      if (test_pixel.colour_type == PNG_COLOR_TYPE_PALETTE)
         test_pixel.sample_depth = 8;
      else
         test_pixel.sample_depth = test_pixel.bit_depth;
      


      test_pixel.have_tRNS = dp->this.is_transparent;
      test_pixel.red_sBIT = test_pixel.green_sBIT = test_pixel.blue_sBIT =
         test_pixel.alpha_sBIT = test_pixel.sample_depth;
      dp->transform_list->mod(dp->transform_list, &test_pixel, pp, dp);
      if (test_pixel.colour_type != dp->output_colour_type)
      {
         char message[128];
         size_t pos = safecat(message, sizeof message, 0, "colour type ");
         pos = safecatn(message, sizeof message, pos, dp->output_colour_type);
         pos = safecat(message, sizeof message, pos, " expected ");
         pos = safecatn(message, sizeof message, pos, test_pixel.colour_type);
         png_error(pp, message);
      }
      if (test_pixel.bit_depth != dp->output_bit_depth)
      {
         char message[128];
         size_t pos = safecat(message, sizeof message, 0, "bit depth ");
         pos = safecatn(message, sizeof message, pos, dp->output_bit_depth);
         pos = safecat(message, sizeof message, pos, " expected ");
         pos = safecatn(message, sizeof message, pos, test_pixel.bit_depth);
         png_error(pp, message);
      }
      


      if (test_pixel.colour_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (test_pixel.sample_depth != 8) 
            png_error(pp, "pngvalid: internal: palette sample depth not 8");
      }
      else if (test_pixel.sample_depth != dp->output_bit_depth)
      {
         char message[128];
         size_t pos = safecat(message, sizeof message, 0,
            "internal: sample depth ");
         pos = safecatn(message, sizeof message, pos, dp->output_bit_depth);
         pos = safecat(message, sizeof message, pos, " expected ");
         pos = safecatn(message, sizeof message, pos, test_pixel.sample_depth);
         png_error(pp, message);
      }
   }
}

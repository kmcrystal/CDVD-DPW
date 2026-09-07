static void
perform_gamma_threshold_tests(png_modifier *pm)
{
   png_byte colour_type = 0;
   png_byte bit_depth = 0;
   unsigned int palette_number = 0;
   



   while (next_format(&colour_type, &bit_depth, &palette_number, 1))
      if (palette_number == 0)
   {
      double test_gamma = 1.0;
      while (test_gamma >= .4)
      {
         


         gamma_threshold_test(pm, colour_type, bit_depth, pm->interlace_type,
            test_gamma, 1/test_gamma);
         test_gamma *= .95;
      }
      
      gamma_threshold_test(pm, colour_type, bit_depth, pm->interlace_type,
          .45455, 2.2);
      if (fail(pm))
         return;
   }
}

static void
perform_gamma_composition_tests(png_modifier *pm, int do_background,
   int expand_16)
{
   png_byte colour_type = 0;
   png_byte bit_depth = 0;
   unsigned int palette_number = 0;
   


   while (next_format(&colour_type, &bit_depth, &palette_number, 1))
      if ((colour_type & PNG_COLOR_MASK_ALPHA) != 0)
   {
      unsigned int i, j;
      
      for (i=0; i<pm->ngamma_tests; ++i) for (j=0; j<pm->ngamma_tests; ++j)
      {
         gamma_composition_test(pm, colour_type, bit_depth, palette_number,
            pm->interlace_type, 1/pm->gammas[i], pm->gammas[j],
            pm->use_input_precision, do_background, expand_16);
         if (fail(pm))
            return;
      }
   }
}

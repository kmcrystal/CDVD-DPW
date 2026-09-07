}
static void perform_gamma_sbit_tests(png_modifier *pm)
{
   png_byte sbit;
   



   for (sbit=pm->sbitlow; sbit<(1<<READ_BDHI); ++sbit)
   {
      png_byte colour_type = 0, bit_depth = 0;
      unsigned int npalette = 0;
      while (next_format(&colour_type, &bit_depth, &npalette, 1))
         if ((colour_type & PNG_COLOR_MASK_ALPHA) == 0 &&
            ((colour_type == 3 && sbit < 8) ||
            (colour_type != 3 && sbit < bit_depth)))
      {
         unsigned int i;
         for (i=0; i<pm->ngamma_tests; ++i)
         {
            unsigned int j;
            for (j=0; j<pm->ngamma_tests; ++j) if (i != j)
            {
               gamma_transform_test(pm, colour_type, bit_depth, npalette,
                  pm->interlace_type, 1/pm->gammas[i], pm->gammas[j],
                  sbit, pm->use_input_precision_sbit, 0 );
               if (fail(pm))
                  return;
            }
         }
      }
   }
}

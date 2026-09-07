#ifdef DO_16BIT
static void perform_gamma_scale16_tests(png_modifier *pm)
{
#  ifndef PNG_MAX_GAMMA_8
#     define PNG_MAX_GAMMA_8 11
#  endif
#  define SBIT_16_TO_8 PNG_MAX_GAMMA_8
   







   unsigned int i, j;
   for (i=0; i<pm->ngamma_tests; ++i)
   {
      for (j=0; j<pm->ngamma_tests; ++j)
      {
         if (i != j &&
             fabs(pm->gammas[j]/pm->gammas[i]-1) >= PNG_GAMMA_THRESHOLD)
         {
            gamma_transform_test(pm, 0, 16, 0, pm->interlace_type,
               1/pm->gammas[i], pm->gammas[j], SBIT_16_TO_8,
               pm->use_input_precision_16to8, 1 );
            if (fail(pm))
               return;
            gamma_transform_test(pm, 2, 16, 0, pm->interlace_type,
               1/pm->gammas[i], pm->gammas[j], SBIT_16_TO_8,
               pm->use_input_precision_16to8, 1 );
            if (fail(pm))
               return;
            gamma_transform_test(pm, 4, 16, 0, pm->interlace_type,
               1/pm->gammas[i], pm->gammas[j], SBIT_16_TO_8,
               pm->use_input_precision_16to8, 1 );
            if (fail(pm))
               return;
            gamma_transform_test(pm, 6, 16, 0, pm->interlace_type,
               1/pm->gammas[i], pm->gammas[j], SBIT_16_TO_8,
               pm->use_input_precision_16to8, 1 );
            if (fail(pm))
               return;
         }
      }
   }
}

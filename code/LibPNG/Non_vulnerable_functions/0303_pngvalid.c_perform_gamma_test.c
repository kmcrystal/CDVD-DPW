static void
perform_gamma_test(png_modifier *pm, int summary)
{
   
   
   unsigned int calculations_use_input_precision =
      pm->calculations_use_input_precision;
#  ifdef PNG_READ_BACKGROUND_SUPPORTED
      double maxout8 = pm->maxout8;
#  endif
   
   if (!pm->this.speed && pm->test_gamma_threshold)
   {
      perform_gamma_threshold_tests(pm);
      if (fail(pm))
         return;
   }
   
   if (pm->test_gamma_transform)
   {
      if (summary)
      {
         fflush(stderr);
         printf("Gamma correction error summary\n\n");
         printf("The printed value is the maximum error in the pixel values\n");
         printf("calculated by the libpng gamma correction code.  The error\n");
         printf("is calculated as the difference between the output pixel\n");
         printf("value (always an integer) and the ideal value from the\n");
         printf("libpng specification (typically not an integer).\n\n");
         printf("Expect this value to be less than .5 for 8 bit formats,\n");
         printf("less than 1 for formats with fewer than 8 bits and a small\n");
         printf("number (typically less than 5) for the 16 bit formats.\n");
         printf("For performance reasons the value for 16 bit formats\n");
         printf("increases when the image file includes an sBIT chunk.\n");
         fflush(stdout);
      }
      init_gamma_errors(pm);
      


      if (pm->test_gamma_expand16)
         pm->calculations_use_input_precision = 1;
      perform_gamma_transform_tests(pm);
      if (!calculations_use_input_precision)
         pm->calculations_use_input_precision = 0;
      if (summary)
         summarize_gamma_errors(pm, 0, 1, 1);
      if (fail(pm))
         return;
   }
   
   if (pm->test_gamma_sbit)
   {
      init_gamma_errors(pm);
      perform_gamma_sbit_tests(pm);
      if (summary)
         summarize_gamma_errors(pm, "sBIT", pm->sbitlow < 8U, 1);
      if (fail(pm))
         return;
   }
#ifdef DO_16BIT 
   if (pm->test_gamma_scale16)
   {
      
      init_gamma_errors(pm);
      perform_gamma_scale16_tests(pm);
      if (summary)
      {
         fflush(stderr);
         printf("\nGamma correction with 16 to 8 bit reduction:\n");
         printf(" 16 bit gray:  %.5f\n", pm->error_gray_16);
         printf(" 16 bit color: %.5f\n", pm->error_color_16);
         fflush(stdout);
      }
      if (fail(pm))
         return;
   }
#endif
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   if (pm->test_gamma_background)
   {
      init_gamma_errors(pm);
      


      if (pm->test_gamma_expand16)
      {
         pm->calculations_use_input_precision = 1;
         pm->maxout8 = .499; 
      }
      perform_gamma_composition_tests(pm, PNG_BACKGROUND_GAMMA_UNIQUE,
         pm->test_gamma_expand16);
      if (!calculations_use_input_precision)
         pm->calculations_use_input_precision = 0;
      pm->maxout8 = maxout8;
      if (summary)
         summarize_gamma_errors(pm, "background", 1, 0);
      if (fail(pm))
         return;
   }
#endif
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
   if (pm->test_gamma_alpha_mode)
   {
      int do_background;
      init_gamma_errors(pm);
      


      if (pm->test_gamma_expand16)
         pm->calculations_use_input_precision = 1;
      for (do_background = ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD;
         do_background <= ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN && !fail(pm);
         ++do_background)
         perform_gamma_composition_tests(pm, do_background,
            pm->test_gamma_expand16);
      if (!calculations_use_input_precision)
         pm->calculations_use_input_precision = 0;
      if (summary)
         summarize_gamma_errors(pm, "alpha mode", 1, 0);
      if (fail(pm))
         return;
   }
#endif
}

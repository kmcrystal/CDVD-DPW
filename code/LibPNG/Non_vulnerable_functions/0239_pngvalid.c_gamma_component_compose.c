static double
gamma_component_compose(int do_background, double input_sample, double alpha,
   double background, int *compose)
{
   switch (do_background)
   {
#ifdef PNG_READ_BACKGROUND_SUPPORTED
      case PNG_BACKGROUND_GAMMA_SCREEN:
      case PNG_BACKGROUND_GAMMA_FILE:
      case PNG_BACKGROUND_GAMMA_UNIQUE:
         
         if (alpha < 1)
         {
            if (alpha > 0)
            {
               input_sample = input_sample * alpha + background * (1-alpha);
               if (compose != NULL)
                  *compose = 1;
            }
            else
               input_sample = background;
         }
         break;
#endif
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      case ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD:
      case ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN:
         



      case ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED:
         



         if (alpha < 1)
         {
            if (alpha > 0)
            {
               input_sample *= alpha;
               if (compose != NULL)
                  *compose = 1;
            }
            else
               input_sample = 0;
         }
         break;
#endif
      default:
         


         UNUSED(alpha)
         UNUSED(background)
         UNUSED(compose)
         break;
   }
   return input_sample;
}

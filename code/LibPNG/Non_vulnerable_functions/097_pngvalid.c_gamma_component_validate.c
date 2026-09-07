static double
gamma_component_validate(PNG_CONST char *name, PNG_CONST validate_info *vi,
    PNG_CONST unsigned int id, PNG_CONST unsigned int od,
    PNG_CONST double alpha ,
    PNG_CONST double background )
{
   PNG_CONST unsigned int isbit = id >> vi->isbit_shift;
   PNG_CONST unsigned int sbit_max = vi->sbit_max;
   PNG_CONST unsigned int outmax = vi->outmax;
   PNG_CONST int do_background = vi->do_background;
   double i;
   



   i = isbit; i /= sbit_max;
   



   if (alpha == 1  || !do_background
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      || do_background == ALPHA_MODE_OFFSET + PNG_ALPHA_PNG
#endif
      || (alpha < 0 
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      && do_background != ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN
#endif
      ))
   {
      




      double encoded_sample = i;
      double encoded_error;
      

      if (alpha >= 0 && vi->gamma_correction > 0)
         encoded_sample = pow(encoded_sample, vi->gamma_correction);
      encoded_sample *= outmax;
      encoded_error = fabs(od-encoded_sample);
      if (encoded_error > vi->dp->maxerrout)
         vi->dp->maxerrout = encoded_error;
      if (encoded_error < vi->maxout_total && encoded_error < vi->outlog)
         return i;
   }
   
   






   {
      double input_sample = i; 
      double output, error, encoded_sample, encoded_error;
      double es_lo, es_hi;
      int compose = 0;           
      int output_is_encoded;     
      int log_max_error = 1;     
      png_const_charp pass = 0;  
      


      if (alpha >= 0)
      {
         int tcompose;
         if (vi->file_inverse > 0)
            input_sample = pow(input_sample, vi->file_inverse);
         
         tcompose = 0;
         input_sample = gamma_component_compose(do_background, input_sample,
            alpha, background, &tcompose);
         if (tcompose)
            compose = 1;
      }
      


      output = od;
      output /= outmax;
      output_is_encoded = vi->screen_gamma > 0;
      if (alpha < 0) 
      {
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
         if (do_background != ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN)
#endif
         {
            


            output_is_encoded = 0;
            log_max_error = 0;
         }
      }
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
      else 
      {
         if (do_background == ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED &&
            alpha < 1) 
         {
            if (alpha > 0) log_max_error = 0;
            output_is_encoded = 0;
         }
      }
#endif
      if (output_is_encoded)
         output = pow(output, vi->screen_gamma);
      


      encoded_sample = input_sample;
      if (output_is_encoded)
         encoded_sample = pow(encoded_sample, vi->screen_inverse);
      encoded_sample *= outmax;
      encoded_error = fabs(od-encoded_sample);
      


      if (log_max_error && encoded_error > vi->dp->maxerrout)
         vi->dp->maxerrout = encoded_error;
      if (encoded_error < vi->maxout_total)
      {
         if (encoded_error < vi->outlog)
            return i;
         


         pass = "less than maxout:\n";
      }
      









      


      error = fabs(input_sample-output);
      if (log_max_error && error > vi->dp->maxerrabs)
         vi->dp->maxerrabs = error;
      


      if (log_max_error && input_sample > .5)
      {
         double percentage_error = error/input_sample;
         if (percentage_error > vi->dp->maxerrpc)
            vi->dp->maxerrpc = percentage_error;
      }
      





      {
         double tmp = input_sample * vi->maxpc;
         if (tmp < vi->maxabs) tmp = vi->maxabs;
         





         if (compose && tmp < vi->maxcalc) tmp = vi->maxcalc;
         


         es_lo = encoded_sample - vi->maxout;
         if (es_lo > 0 && input_sample-tmp > 0)
         {
            double low_value = input_sample-tmp;
            if (output_is_encoded)
               low_value = pow(low_value, vi->screen_inverse);
            low_value *= outmax;
            if (low_value < es_lo) es_lo = low_value;
            
            es_lo = ceil(es_lo / vi->outquant - .5) * vi->outquant;
         }
         else
            es_lo = 0;
         es_hi = encoded_sample + vi->maxout;
         if (es_hi < outmax && input_sample+tmp < 1)
         {
            double high_value = input_sample+tmp;
            if (output_is_encoded)
               high_value = pow(high_value, vi->screen_inverse);
            high_value *= outmax;
            if (high_value > es_hi) es_hi = high_value;
            es_hi = floor(es_hi / vi->outquant + .5) * vi->outquant;
         }
         else
            es_hi = outmax;
      }
      



      if (od >= es_lo && od <= es_hi)
      {
         
         if (encoded_error < vi->outlog)
            return i;
         if (pass == 0)
            pass = "within digitization limits:\n";
      }
      {
         


         double is_lo, is_hi;
         



         if (pass == 0 && vi->use_input_precision && vi->dp->sbit)
         {
            










#           if PNG_LIBPNG_VER < 10700
#              define SBIT_ERROR .5
#           else
#              define SBIT_ERROR 1.
#           endif
            double tmp = (isbit - SBIT_ERROR)/sbit_max;
            if (tmp <= 0)
               tmp = 0;
            else if (alpha >= 0 && vi->file_inverse > 0 && tmp < 1)
               tmp = pow(tmp, vi->file_inverse);
            tmp = gamma_component_compose(do_background, tmp, alpha, background,
               NULL);
            if (output_is_encoded && tmp > 0 && tmp < 1)
               tmp = pow(tmp, vi->screen_inverse);
            is_lo = ceil(outmax * tmp - vi->maxout_total);
            if (is_lo < 0)
               is_lo = 0;
            tmp = (isbit + SBIT_ERROR)/sbit_max;
            if (tmp >= 1)
               tmp = 1;
            else if (alpha >= 0 && vi->file_inverse > 0 && tmp < 1)
               tmp = pow(tmp, vi->file_inverse);
            tmp = gamma_component_compose(do_background, tmp, alpha, background,
               NULL);
            if (output_is_encoded && tmp > 0 && tmp < 1)
               tmp = pow(tmp, vi->screen_inverse);
            is_hi = floor(outmax * tmp + vi->maxout_total);
            if (is_hi > outmax)
               is_hi = outmax;
            if (!(od < is_lo || od > is_hi))
            {
               if (encoded_error < vi->outlog)
                  return i;
               pass = "within input precision limits:\n";
            }
            







#           ifndef PNG_READ_16_TO_8_ACCURATE_SCALE_SUPPORTED
#              if PNG_LIBPNG_VER < 10504
                  





                  if (pass == 0 && alpha < 0 && vi->scale16 && vi->sbit > 8 &&
                     vi->sbit + vi->isbit_shift == 16)
                  {
                     tmp = ((id >> 8) - .5)/255;
                     if (tmp > 0)
                     {
                        is_lo = ceil(outmax * tmp - vi->maxout_total);
                        if (is_lo < 0) is_lo = 0;
                     }
                     else
                        is_lo = 0;
                     tmp = ((id >> 8) + .5)/255;
                     if (tmp < 1)
                     {
                        is_hi = floor(outmax * tmp + vi->maxout_total);
                        if (is_hi > outmax) is_hi = outmax;
                     }
                     else
                        is_hi = outmax;
                     if (!(od < is_lo || od > is_hi))
                     {
                        if (encoded_error < vi->outlog)
                           return i;
                        pass = "within 8 bit limits:\n";
                     }
                  }
#              endif
#           endif
         }
         else 
            is_lo = es_lo, is_hi = es_hi;
         



         {
            size_t pos = 0;
            


            int precision = (outmax >= 1000 ? 6 : 3);
            int use_input=1, use_background=0, do_compose=0;
            char msg[256];
            if (pass != 0)
               pos = safecat(msg, sizeof msg, pos, "\n\t");
            


            switch (do_background)
            {
#           ifdef PNG_READ_BACKGROUND_SUPPORTED
               case PNG_BACKGROUND_GAMMA_SCREEN:
               case PNG_BACKGROUND_GAMMA_FILE:
               case PNG_BACKGROUND_GAMMA_UNIQUE:
                  use_background = (alpha >= 0 && alpha < 1);
                  
#           endif
#           ifdef PNG_READ_ALPHA_MODE_SUPPORTED
               case ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD:
               case ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN:
               case ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED:
#           endif 
               do_compose = (alpha > 0 && alpha < 1);
               use_input = (alpha != 0);
               break;
            default:
               break;
            }
            
            if (compose != do_compose)
               png_error(vi->pp, "internal error (compose)");
            
            pos = safecat(msg, sizeof msg, pos, name);
            pos = safecat(msg, sizeof msg, pos, "(");
            pos = safecatn(msg, sizeof msg, pos, id);
            if (use_input || pass != 0)
            {
               if (isbit != id)
               {
                  
                  pos = safecat(msg, sizeof msg, pos, ", sbit(");
                  pos = safecatn(msg, sizeof msg, pos, vi->sbit);
                  pos = safecat(msg, sizeof msg, pos, "): ");
                  pos = safecatn(msg, sizeof msg, pos, isbit);
               }
               pos = safecat(msg, sizeof msg, pos, "/");
               
               pos = safecatn(msg, sizeof msg, pos, vi->sbit_max);
            }
            pos = safecat(msg, sizeof msg, pos, ")");
            


            if (compose || pass != 0)
            {
               




               if (use_input || pass != 0)
               {
                  if (vi->file_inverse > 0)
                  {
                     pos = safecat(msg, sizeof msg, pos, "^");
                     pos = safecatd(msg, sizeof msg, pos, vi->file_inverse, 2);
                  }
                  else
                     pos = safecat(msg, sizeof msg, pos, "[linear]");
                  pos = safecat(msg, sizeof msg, pos, "*(alpha)");
                  pos = safecatd(msg, sizeof msg, pos, alpha, precision);
               }
               



               if (use_background)
               {
                  pos = safecat(msg, sizeof msg, pos, use_input ? "+" : " ");
                  pos = safecat(msg, sizeof msg, pos, "(background)");
                  pos = safecatd(msg, sizeof msg, pos, background, precision);
                  pos = safecat(msg, sizeof msg, pos, "*");
                  pos = safecatd(msg, sizeof msg, pos, 1-alpha, precision);
               }
            }
            



            if (compose || alpha < 0 || pass != 0)
            {
               pos = safecat(msg, sizeof msg, pos,
                  pass != 0 ? " =\n\t" : " = ");
               pos = safecatd(msg, sizeof msg, pos, input_sample, precision);
               pos = safecat(msg, sizeof msg, pos, " (libpng: ");
               pos = safecatd(msg, sizeof msg, pos, output, precision);
               pos = safecat(msg, sizeof msg, pos, ")");
               
               if (output_is_encoded)
               {
                  pos = safecat(msg, sizeof msg, pos, " ^");
                  pos = safecatd(msg, sizeof msg, pos, vi->screen_inverse, 2);
                  pos = safecat(msg, sizeof msg, pos, "(to screen) =");
               }
               else
                  pos = safecat(msg, sizeof msg, pos, " [screen is linear] =");
            }
            if ((!compose && alpha >= 0) || pass != 0)
            {
               if (pass != 0) 
                  pos = safecat(msg, sizeof msg, pos, "\n\t[overall:");
               




               if (vi->gamma_correction > 0)
               {
                  pos = safecat(msg, sizeof msg, pos, " ^");
                  pos = safecatd(msg, sizeof msg, pos, vi->gamma_correction, 2);
                  pos = safecat(msg, sizeof msg, pos, "(gamma correction) =");
               }
               else
                  pos = safecat(msg, sizeof msg, pos,
                     " [no gamma correction] =");
               if (pass != 0)
                  pos = safecat(msg, sizeof msg, pos, "]");
            }
            


            pos = safecat(msg, sizeof msg, pos, pass != 0 ? "\n\t" : " ");
            pos = safecatd(msg, sizeof msg, pos, is_lo, 1);
            pos = safecat(msg, sizeof msg, pos, " < ");
            pos = safecatd(msg, sizeof msg, pos, encoded_sample, 1);
            pos = safecat(msg, sizeof msg, pos, " (libpng: ");
            pos = safecatn(msg, sizeof msg, pos, od);
            pos = safecat(msg, sizeof msg, pos, ")");
            pos = safecat(msg, sizeof msg, pos, "/");
            pos = safecatn(msg, sizeof msg, pos, outmax);
            pos = safecat(msg, sizeof msg, pos, " < ");
            pos = safecatd(msg, sizeof msg, pos, is_hi, 1);
            if (pass == 0) 
            {
#              ifdef PNG_WARNINGS_SUPPORTED
                  png_warning(vi->pp, msg);
#              else
                  store_warning(vi->pp, msg);
#              endif
            }
            else 
               store_verbose(&vi->dp->pm->this, vi->pp, pass, msg);
         }
      }
   }
   return i;
}

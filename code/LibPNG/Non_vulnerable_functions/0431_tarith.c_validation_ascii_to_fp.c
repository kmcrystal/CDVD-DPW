static int verbose = 0;
int validation_ascii_to_fp(int count, int argc, char **argv)
{
   int    showall = 0;
   double max_error=2;      
   double max_error_abs=17; 
   double max = 0;
   double max_abs = 0;
   double test = 0; 
   int    precision = 5;
   int    nonfinite = 0;
   int    finite = 0;
   int    ok = 0;
   int    failcount = 0;
   int    minorarith = 0;
   while (--argc > 0)
      if (strcmp(*++argv, "-a") == 0)
         showall = 1;
      else if (strcmp(*argv, "-e") == 0 && argc > 0)
      {
         --argc;
         max_error = atof(*++argv);
      }
      else if (strcmp(*argv, "-E") == 0 && argc > 0)
      {
         --argc;
         max_error_abs = atof(*++argv);
      }
      else
      {
         fprintf(stderr, "unknown argument %s\n", *argv);
         return 1;
      }
   do
   {
      png_size_t index;
      int state, failed = 0;
      char buffer[64];
      if (isfinite(test))
         ++finite;
      else
         ++nonfinite;
      if (verbose)
         fprintf(stderr, "%.*g %d\n", DBL_DIG, test, precision);
      
      memset(buffer, 71, sizeof buffer);
      png_ascii_from_fp(0, buffer, precision+10, test, precision);
      


      if (buffer[precision+7] != 71)
      {
         fprintf(stderr, "%g[%d] -> '%s'[%lu] buffer overflow\n", test,
            precision, buffer, (unsigned long)strlen(buffer));
         failed = 1;
      }
      


      state = 0;
      index = 0;
      if (!isfinite(test))
      {
         
         if (test >= 0 && strcmp(buffer, "inf") ||
             test <  0 && strcmp(buffer, "-inf"))
         {
            fprintf(stderr, "%g[%d] -> '%s' but expected 'inf'\n", test,
               precision, buffer);
            failed = 1;
         }
      }
      else if (!png_check_fp_number(buffer, precision+10, &state, &index) ||
          buffer[index] != 0)
      {
         fprintf(stderr, "%g[%d] -> '%s' but has bad format ('%c')\n", test,
         precision, buffer, buffer[index]);
         failed = 1;
      }
      else if (PNG_FP_IS_NEGATIVE(state) && !(test < 0))
      {
         fprintf(stderr, "%g[%d] -> '%s' but negative value not so reported\n",
            test, precision, buffer);
         failed = 1;
         assert(!PNG_FP_IS_ZERO(state));
         assert(!PNG_FP_IS_POSITIVE(state));
      }
      else if (PNG_FP_IS_ZERO(state) && !(test == 0))
      {
         fprintf(stderr, "%g[%d] -> '%s' but zero value not so reported\n",
            test, precision, buffer);
         failed = 1;
         assert(!PNG_FP_IS_NEGATIVE(state));
         assert(!PNG_FP_IS_POSITIVE(state));
      }
      else if (PNG_FP_IS_POSITIVE(state) && !(test > 0))
      {
         fprintf(stderr, "%g[%d] -> '%s' but postive value not so reported\n",
            test, precision, buffer);
         failed = 1;
         assert(!PNG_FP_IS_NEGATIVE(state));
         assert(!PNG_FP_IS_ZERO(state));
      }
      else
      {
         
         double out = atof(buffer);
         double change = fabs((out - test)/test);
         double allow = .5/pow(10,
            (precision >= DBL_DIG) ? DBL_DIG-1 : precision-1);
         






         if (change >= allow && (isfinite(out) ||
             fabs(test/DBL_MAX) <= 1-allow))
         {
            double percent = (precision >= DBL_DIG) ? max_error_abs : max_error;
            double allowp = (change-allow)*100/allow;
            if (precision >= DBL_DIG)
            {
               if (max_abs < allowp) max_abs = allowp;
            }
            else
            {
               if (max < allowp) max = allowp;
            }
            if (showall || allowp >= percent)
            {
               fprintf(stderr,
                  "%.*g[%d] -> '%s' -> %.*g number changed (%g > %g (%d%%))\n",
                  DBL_DIG, test, precision, buffer, DBL_DIG, out, change, allow,
                  (int)round(allowp));
               failed = 1;
            }
            else
               ++minorarith;
         }
      }
      if (failed)
         ++failcount;
      else
         ++ok;
skip:
      
      precision = rand();
      if (precision & 1) test = -test;
      precision >>= 1;
      
      if (test == 0 || !isfinite(test))
         test = precision+1;
      else
      {
         
         int exponent = precision % (DBL_MAX_EXP - DBL_MIN_EXP) + DBL_MIN_EXP;
         int tmp;
         test = frexp(test * rand(), &tmp);
         test = ldexp(test, exponent);
         precision >>= 8; 
      }
      


      precision = (precision & 0x1f) + 1;
   }
   while (--count);
   printf("Tested %d finite values, %d non-finite, %d OK (%d failed) %d minor "
      "arithmetic errors\n", finite, nonfinite, ok, failcount, minorarith);
   printf(" Error with >=%d digit precision %.2f%%\n", DBL_DIG, max_abs);
   printf(" Error with < %d digit precision %.2f%%\n", DBL_DIG, max);
   return 0;
}

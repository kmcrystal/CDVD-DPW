}
static int perform_one_test(FILE *fp, int nfiles)
{
   int i;
   struct timespec before, after;
   
   rewind(fp);
   if (mytime(&before))
   {
      for (i=0; i<nfiles; ++i)
      {
         if (read_png(fp))
         {
            if (ferror(fp))
            {
               perror("temporary file");
               fprintf(stderr, "file %d: error reading PNG data\n", i);
               return 0;
            }
         }
         else
         {
            perror("temporary file");
            fprintf(stderr, "file %d: error from libpng\n", i);
            return 0;
         }
      }
   }
   else
      return 0;
   if (mytime(&after))
   {
      


      unsigned long s = after.tv_sec - before.tv_sec;
      long ns = after.tv_nsec - before.tv_nsec;
      if (ns < 0)
      {
         --s;
         ns += 1000000000;
         if (ns < 0)
         {
            fprintf(stderr, "timepng: bad clock from kernel\n");
            return 0;
         }
      }
      printf("%lu.%.9ld\n", s, ns);
      fflush(stdout);
      if (ferror(stdout))
      {
         fprintf(stderr, "timepng: error writing output\n");
         return 0;
      }
      
      return 1;
   }
   else
      return 0;
}

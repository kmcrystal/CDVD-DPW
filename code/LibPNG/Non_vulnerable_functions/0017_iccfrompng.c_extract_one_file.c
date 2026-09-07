static int
extract_one_file(const char *filename)
{
   int result = 0;
   FILE *fp = fopen(filename, "rb");
   if (fp != NULL)
   {
      png_uint_32 proflen = 0;
      png_bytep profile = extract(fp, &proflen);
      if (profile != NULL && profile != no_profile)
      {
         size_t len;
         char *output;
         {
            const char *ep = strrchr(filename, '.');
            if (ep != NULL)
               len = ep-filename;
            else
               len = strlen(filename);
         }
         output = malloc(len + 5);
         if (output != NULL)
         {
            FILE *of;
            memcpy(output, filename, len);
            strcpy(output+len, ".icc");
            of = fopen(output, "wb");
            if (of != NULL)
            {
               if (fwrite(profile, proflen, 1, of) == 1 &&
                  fflush(of) == 0 &&
                  fclose(of) == 0)
               {
                  if (verbose)
                     printf("%s -> %s\n", filename, output);
                  
                  result = 1;
               }
               else
               {
                  fprintf(stderr, "%s: error writing profile\n", output);
                  if (remove(output))
                     fprintf(stderr, "%s: could not remove file\n", output);
               }
            }
            else
               fprintf(stderr, "%s: failed to open output file\n", output);
            free(output);
         }
         else
            fprintf(stderr, "%s: OOM allocating string!\n", filename);
         free(profile);
      }
      else if (verbose && profile == no_profile)
	printf("%s has no profile\n", filename);
   }
   else
      fprintf(stderr, "%s: could not open file\n", filename);
   return result;
}

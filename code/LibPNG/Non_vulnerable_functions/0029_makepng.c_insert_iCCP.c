static void
insert_iCCP(png_structp png_ptr, png_infop info_ptr, int nparams,
   png_charpp params)
{
   png_bytep profile = NULL;
   png_uint_32 proflen = 0;
   int result;
   check_param_count(nparams, 2);
   switch (params[1][0])
   {
      case '<':
         {
            png_size_t filelen = load_file(params[1]+1, &profile);
            if (filelen > 0xfffffffc) 
            {
               fprintf(stderr, "%s: file too long (%lu) for an ICC profile\n",
                  params[1]+1, (unsigned long)filelen);
               exit(1);
            }
            proflen = (png_uint_32)filelen;
         }
         break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
         {
            png_size_t fake_len = load_fake(params[1], &profile);
            if (fake_len > 0) 
            {
               if (fake_len > 0xffffffff) 
               {
                  fprintf(stderr,
                     "%s: fake data too long (%lu) for an ICC profile\n",
                     params[1], (unsigned long)fake_len);
                  exit(1);
               }
               proflen = (png_uint_32)(fake_len & ~3U);
               
               png_save_uint_32(profile, proflen);
               break;
            }
         }
      default:
         fprintf(stderr, "--insert iCCP \"%s\": unrecognized\n", params[1]);
         fprintf(stderr, "  use '<' to read a file: \"<filename\"\n");
         exit(1);
   }
   result = 1;
   if (proflen & 3)
   {
      fprintf(stderr,
         "makepng: --insert iCCP %s: profile length made a multiple of 4\n",
         params[1]);
      


      while (proflen & 3)
         profile[proflen++] = 0;
   }
   if (profile != NULL && proflen > 3)
   {
      png_uint_32 prof_header = png_get_uint_32(profile);
      if (prof_header != proflen)
      {
         fprintf(stderr, "--insert iCCP %s: profile length field wrong:\n",
            params[1]);
         fprintf(stderr, "  actual %lu, recorded value %lu (corrected)\n",
            (unsigned long)proflen, (unsigned long)prof_header);
         png_save_uint_32(profile, proflen);
      }
   }
   if (result && profile != NULL && proflen >=4)
      png_set_iCCP(png_ptr, info_ptr, params[0], PNG_COMPRESSION_TYPE_BASE,
         profile, proflen);
   if (profile)
      free(profile);
   if (!result)
      exit(1);
}

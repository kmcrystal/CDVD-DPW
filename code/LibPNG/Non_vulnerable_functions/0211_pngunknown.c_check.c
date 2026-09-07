static int
check(FILE *fp, int argc, const char **argv, png_uint_32p flags,
   display *d, int set_callback)
{
   int i, npasses, ipass;
   png_uint_32 height;
   d->keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
   d->before_IDAT = 0;
   d->after_IDAT = 0;
   


   d->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, d, error,
      warning);
   if (d->png_ptr == NULL)
   {
      fprintf(stderr, "%s(%s): could not allocate png struct\n", d->file,
         d->test);
      
      exit(1);
   }
   d->info_ptr = png_create_info_struct(d->png_ptr);
   d->end_ptr = png_create_info_struct(d->png_ptr);
   if (d->info_ptr == NULL || d->end_ptr == NULL)
   {
      fprintf(stderr, "%s(%s): could not allocate png info\n", d->file,
         d->test);
      clean_display(d);
      exit(1);
   }
   png_init_io(d->png_ptr, fp);
#  ifdef PNG_READ_USER_CHUNKS_SUPPORTED
      


      if (set_callback)
         png_set_read_user_chunk_fn(d->png_ptr, d, read_callback);
#  else
      UNUSED(set_callback)
#  endif
   



   for (i=0; i<argc; ++i)
   {
      const char *equals = strchr(argv[i], '=');
      if (equals != NULL)
      {
         int chunk, option;
         if (strcmp(equals+1, "default") == 0)
            option = PNG_HANDLE_CHUNK_AS_DEFAULT;
         else if (strcmp(equals+1, "discard") == 0)
            option = PNG_HANDLE_CHUNK_NEVER;
         else if (strcmp(equals+1, "if-safe") == 0)
            option = PNG_HANDLE_CHUNK_IF_SAFE;
         else if (strcmp(equals+1, "save") == 0)
            option = PNG_HANDLE_CHUNK_ALWAYS;
         else
         {
            fprintf(stderr, "%s(%s): %s: unrecognized chunk option\n", d->file,
               d->test, argv[i]);
            display_exit(d);
         }
         switch (equals - argv[i])
         {
            case 4: 
               chunk = find(argv[i]);
               if (chunk >= 0)
               {
                  




                  png_byte name[5];
                  memcpy(name, chunk_info[chunk].name, 5);
                  png_set_keep_unknown_chunks(d->png_ptr, option, name, 1);
                  chunk_info[chunk].keep = option;
                  continue;
               }
               break;
            case 7: 
               if (memcmp(argv[i], "default", 7) == 0)
               {
                  png_set_keep_unknown_chunks(d->png_ptr, option, NULL, 0);
                  d->keep = option;
                  continue;
               }
               break;
            case 3: 
               if (memcmp(argv[i], "all", 3) == 0)
               {
                  png_set_keep_unknown_chunks(d->png_ptr, option, NULL, -1);
                  d->keep = option;
                  for (chunk = 0; chunk < NINFO; ++chunk)
                     if (chunk_info[chunk].all)
                        chunk_info[chunk].keep = option;
                  continue;
               }
               break;
            default: 
               break;
         }
      }
      fprintf(stderr, "%s(%s): %s: unrecognized chunk argument\n", d->file,
         d->test, argv[i]);
      display_exit(d);
   }
   png_read_info(d->png_ptr, d->info_ptr);
   switch (png_get_interlace_type(d->png_ptr, d->info_ptr))
   {
      case PNG_INTERLACE_NONE:
         npasses = 1;
         break;
      case PNG_INTERLACE_ADAM7:
         npasses = PNG_INTERLACE_ADAM7_PASSES;
         break;
      default:
         
         fprintf(stderr, "%s(%s): invalid interlace type\n", d->file, d->test);
         clean_display(d);
         exit(1);
   }
   


   if (chunk_info[0].keep == PNG_HANDLE_CHUNK_AS_DEFAULT)
   {
      png_start_read_image(d->png_ptr);
      height = png_get_image_height(d->png_ptr, d->info_ptr);
      if (npasses > 1)
      {
         png_uint_32 width = png_get_image_width(d->png_ptr, d->info_ptr);
         for (ipass=0; ipass<npasses; ++ipass)
         {
            png_uint_32 wPass = PNG_PASS_COLS(width, ipass);
            if (wPass > 0)
            {
               png_uint_32 y;
               for (y=0; y<height; ++y) if (PNG_ROW_IN_INTERLACE_PASS(y, ipass))
                  png_read_row(d->png_ptr, NULL, NULL);
            }
         }
      } 
      else 
      {
         png_uint_32 y;
         for (y=0; y<height; ++y)
            png_read_row(d->png_ptr, NULL, NULL);
      }
   }
   png_read_end(d->png_ptr, d->end_ptr);
   flags[0] = get_valid(d, d->info_ptr);
   flags[1] = get_unknown(d, d->info_ptr, 0);
   
   flags[chunk_info[0].keep != PNG_HANDLE_CHUNK_AS_DEFAULT] |=
      PNG_INFO_IDAT;
   flags[2] = get_valid(d, d->end_ptr);
   flags[3] = get_unknown(d, d->end_ptr, 1);
   clean_display(d);
   return d->keep;
}

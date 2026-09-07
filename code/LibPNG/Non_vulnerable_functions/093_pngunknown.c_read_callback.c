static int PNGCBAPI
read_callback(png_structp pp, png_unknown_chunkp pc)
{
   


   display *d = voidcast(display*, png_get_user_chunk_ptr(pp));
   int chunk = findb(pc->name);
   int keep, discard;
   if (chunk < 0) 
      keep = d->keep;
   else
   {
      keep = chunk_info[chunk].keep;
      if (keep == PNG_HANDLE_CHUNK_AS_DEFAULT)
      {
         


         if (chunk_info[chunk].unknown)
            keep = d->keep;
         else
            keep = PNG_HANDLE_CHUNK_NEVER;
      }
   }
   switch (keep)
   {
      default:
         fprintf(stderr, "%s(%s): %d: unrecognized chunk option\n", d->file,
            d->test, chunk_info[chunk].keep);
         display_exit(d);
      case PNG_HANDLE_CHUNK_AS_DEFAULT:
      case PNG_HANDLE_CHUNK_NEVER:
         discard = 1;
         break;
      case PNG_HANDLE_CHUNK_IF_SAFE:
      case PNG_HANDLE_CHUNK_ALWAYS:
         discard = 0;
         break;
   }
   


   if (chunk >= 0) if (!discard) 
   {
      png_uint_32 flag = chunk_info[chunk].flag;
      if (pc->location & PNG_AFTER_IDAT)
         d->after_IDAT |= flag;
      else
         d->before_IDAT |= flag;
   }
   


#  ifdef PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
      return discard;
#  else
      return 1; 
#  endif
}

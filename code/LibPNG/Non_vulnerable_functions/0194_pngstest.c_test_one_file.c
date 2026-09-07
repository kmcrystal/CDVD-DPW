static int
test_one_file(const char *file_name, format_list *formats, png_uint_32 opts,
   int stride_extra, int log_pass)
{
   int result;
   Image image;
   newimage(&image);
   initimage(&image, opts, file_name, stride_extra);
   result = read_one_file(&image);
   if (result)
      result = testimage(&image, opts, formats);
   freeimage(&image);
   
   fflush(stderr);
   if (log_pass)
   {
      if (result)
         printf("PASS:");
      else
         printf("FAIL:");
#     ifndef PNG_SIMPLIFIED_WRITE_SUPPORTED
         printf(" (no write)");
#     endif
      print_opts(opts);
      printf(" %s\n", file_name);
      
      fflush(stdout);
   }
   else if (!result)
      exit(1);
   return result;
}

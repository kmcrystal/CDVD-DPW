static void
summarize_gamma_errors(png_modifier *pm, png_const_charp who, int low_bit_depth,
   int indexed)
{
   fflush(stderr);
   if (who)
      printf("\nGamma correction with %s:\n", who);
   else
      printf("\nBasic gamma correction:\n");
   if (low_bit_depth)
   {
      print_one(" 2 bit gray: ", pm->error_gray_2);
      print_one(" 4 bit gray: ", pm->error_gray_4);
      print_one(" 8 bit gray: ", pm->error_gray_8);
      print_one(" 8 bit color:", pm->error_color_8);
      if (indexed)
         print_one(" indexed:    ", pm->error_indexed);
   }
   print_one("16 bit gray: ", pm->error_gray_16);
   print_one("16 bit color:", pm->error_color_16);
   fflush(stdout);
}

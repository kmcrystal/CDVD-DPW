static void
perform_one_test(FILE *fp, int argc, const char **argv,
   png_uint_32 *default_flags, display *d, int set_callback)
{
   int def;
   png_uint_32 flags[2][4];
   rewind(fp);
   clear_keep();
   memcpy(flags[0], default_flags, sizeof flags[0]);
   def = check(fp, argc, argv, flags[1], d, set_callback);
   




   check_error(d, (flags[0][0]|flags[0][2]) & (flags[0][1]|flags[0][3]),
      "chunk handled inconsistently in count tests");
   check_error(d, (flags[1][0]|flags[1][2]) & (flags[1][1]|flags[1][3]),
      "chunk handled inconsistently in option tests");
   




   {
      png_uint_32 test;
      test = flags[1][0] & ~flags[0][0];
      check_error(d, test, "new known chunk before IDAT");
      test = flags[1][1] & ~(flags[0][0] | flags[0][1]);
      check_error(d, test, "new unknown chunk before IDAT");
      test = flags[1][2] & ~flags[0][2];
      check_error(d, test, "new known chunk after IDAT");
      test = flags[1][3] & ~(flags[0][2] | flags[0][3]);
      check_error(d, test, "new unknown chunk after IDAT");
   }
   



   check_handling(d, def, flags[0][0] | flags[0][1], flags[1][0], flags[1][1],
      "before IDAT", set_callback);
   check_handling(d, def, flags[0][2] | flags[0][3], flags[1][2], flags[1][3],
      "after IDAT", set_callback);
}

static void
check_interlace_type(int PNG_CONST interlace_type)
{
   if (interlace_type != PNG_INTERLACE_NONE)
   {
      


      fprintf(stderr, "pngvalid: no interlace support\n");
      exit(99);
   }
}

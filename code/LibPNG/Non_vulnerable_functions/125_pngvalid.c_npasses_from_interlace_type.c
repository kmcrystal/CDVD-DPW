static int
npasses_from_interlace_type(png_const_structp pp, int interlace_type)
{
   switch (interlace_type)
   {
   default:
      png_error(pp, "invalid interlace type");
   case PNG_INTERLACE_NONE:
      return 1;
   case PNG_INTERLACE_ADAM7:
      return PNG_INTERLACE_ADAM7_PASSES;
   }
}

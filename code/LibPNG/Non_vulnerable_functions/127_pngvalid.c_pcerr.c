}
static double pcerr(PNG_CONST png_modifier *pm, int in_depth, int out_depth)
{
   


   if (pm->assume_16_bit_calculations ||
      (pm->calculations_use_input_precision ? in_depth : out_depth) == 16)
      return pm->maxpc16 * .01;
   else
      return pm->maxpc8 * .01;
}

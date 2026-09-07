 */
static double outerr(PNG_CONST png_modifier *pm, int in_depth, int out_depth)
{
   






   if (out_depth == 2)
      return .73182-.5;
   if (out_depth == 4)
      return .90644-.5;
   if ((pm->calculations_use_input_precision ? in_depth : out_depth) == 16)
      return pm->maxout16;
   


   else if (out_depth == 16)
      return pm->maxout8 * 257;
   else
      return pm->maxout8;
}

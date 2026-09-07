 */
static double outlog(PNG_CONST png_modifier *pm, int in_depth, int out_depth)
{
   


   if (out_depth <= 8)
   {
      if (pm->log8 == 0) 
         return 256;
      if (out_depth < 8)
         return pm->log8 / 255 * ((1<<out_depth)-1);
      return pm->log8;
   }
   if ((pm->calculations_use_input_precision ? in_depth : out_depth) == 16)
   {
      if (pm->log16 == 0)
         return 65536;
      return pm->log16;
   }
   


   if (pm->log8 == 0)
      return 65536;
   return pm->log8 * 257;
}

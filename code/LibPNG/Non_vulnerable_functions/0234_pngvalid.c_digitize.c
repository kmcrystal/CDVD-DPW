#if DIGITIZE
static double digitize(double value, int depth, int do_round)
{
   





   PNG_CONST unsigned int digitization_factor = (1U << depth) -1;
   


   if (value <= 0)
      value = 0;
   else if (value >= 1)
      value = 1;
   value *= digitization_factor;
   if (do_round) value += .5;
   return floor(value)/digitization_factor;
}

static unsigned int
modifier_total_encodings(PNG_CONST png_modifier *pm)
{
   return 1 +                 
      pm->ngammas +           
      pm->nencodings +        
      







      ((pm->bit_depth == 16 || pm->assume_16_bit_calculations) ?
         pm->nencodings : 0); 
}

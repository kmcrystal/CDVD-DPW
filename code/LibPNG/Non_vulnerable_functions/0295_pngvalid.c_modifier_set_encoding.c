static void
modifier_set_encoding(png_modifier *pm)
{
   



   pm->current_gamma = 0;
   pm->current_encoding = 0;
   pm->encoding_ignored = 0; 
   
   if (pm->encoding_counter > 0)
   {
      


      if (pm->encoding_counter <= pm->ngammas)
         pm->current_gamma = 1/pm->gammas[pm->encoding_counter-1];
      else
      {
         unsigned int i = pm->encoding_counter - pm->ngammas;
         if (i >= pm->nencodings)
         {
            i %= pm->nencodings;
            pm->current_gamma = 1; 
         }
         else
            pm->current_gamma = pm->encodings[i].gamma;
         pm->current_encoding = pm->encodings + i;
      }
   }
}

static void
modifier_encoding_iterate(png_modifier *pm)
{
   if (!pm->repeat && 
      pm->test_uses_encoding) 
   {
      if (pm->test_exhaustive)
      {
         if (++pm->encoding_counter >= modifier_total_encodings(pm))
            pm->encoding_counter = 0; 
      }
      else
      {
         


         if (pm->encoding_counter == 0)
            pm->encoding_counter = random_mod(modifier_total_encodings(pm)-1)+1;
         else
            pm->encoding_counter = 0;
      }
      if (pm->encoding_counter > 0)
         pm->repeat = 1;
   }
   else if (!pm->repeat)
      pm->encoding_counter = 0;
}

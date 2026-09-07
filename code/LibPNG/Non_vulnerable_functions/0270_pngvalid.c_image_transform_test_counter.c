static int
image_transform_test_counter(png_uint_32 counter, unsigned int max)
{
   


   image_transform *next = image_transform_first;
   while (next != &image_transform_end)
   {
      
      counter >>= 1;
      
      if (max > 1 && next->local_use < max)
         return 1;
      next = next->list;
   }
   return max <= 1 && counter == 0;
}

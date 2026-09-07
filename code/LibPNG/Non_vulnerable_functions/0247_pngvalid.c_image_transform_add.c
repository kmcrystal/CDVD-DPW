static png_uint_32
image_transform_add(PNG_CONST image_transform **this, unsigned int max,
   png_uint_32 counter, char *name, size_t sizeof_name, size_t *pos,
   png_byte colour_type, png_byte bit_depth)
{
   for (;;) 
   {
      png_uint_32 mask;
      image_transform *list;
      



      if (counter == 0) 
      {
         image_transform_reset_count();
         if (max <= 1)
            counter = 1;
         else
            counter = random_32();
      }
      else 
      {
         switch (max)
         {
            case 0:  ++counter; break;
            case 1:  counter <<= 1; break;
            default: counter = random_32(); break;
         }
      }
      
      *this = &image_transform_end;
      list = image_transform_first;
      mask = 1;
      
      while (list != &image_transform_end)
      {
         if ((counter & mask) != 0 && list->enable &&
             (max == 0 || list->local_use < max))
         {
            
            if (list->add(list, this, colour_type, bit_depth) || max == 0)
            {
               
               *pos = safecat(name, sizeof_name, *pos, " +");
               *pos = safecat(name, sizeof_name, *pos, list->name);
            }
            else
            {
               
               *this = list->next;
               list->next = 0;
               


               list->local_use = max;
            }
         }
         mask <<= 1;
         list = list->list;
      }
      
      if (*this != &image_transform_end)
         return counter;
      
      if (!image_transform_test_counter(counter, max))
         return 0;
   }
}
static png_uint_32
image_transform_add(PNG_CONST image_transform **this, unsigned int max,
   png_uint_32 counter, char *name, size_t sizeof_name, size_t *pos,
   png_byte colour_type, png_byte bit_depth)
{
   for (;;) 
   {
      png_uint_32 mask;
      image_transform *list;
      



      if (counter == 0) 
      {
         image_transform_reset_count();
         if (max <= 1)
            counter = 1;
         else
            counter = random_32();
      }
      else 
      {
         switch (max)
         {
            case 0:  ++counter; break;
            case 1:  counter <<= 1; break;
            default: counter = random_32(); break;
         }
      }
      
      *this = &image_transform_end;
      list = image_transform_first;
      mask = 1;
      
      while (list != &image_transform_end)
      {
         if ((counter & mask) != 0 && list->enable &&
             (max == 0 || list->local_use < max))
         {
            
            if (list->add(list, this, colour_type, bit_depth) || max == 0)
            {
               
               *pos = safecat(name, sizeof_name, *pos, " +");
               *pos = safecat(name, sizeof_name, *pos, list->name);
            }
            else
            {
               
               *this = list->next;
               list->next = 0;
               


               list->local_use = max;
            }
         }
         mask <<= 1;
         list = list->list;
      }
      
      if (*this != &image_transform_end)
         return counter;
      
      if (!image_transform_test_counter(counter, max))
         return 0;
   }

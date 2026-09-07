static void
perform_transform_test(png_modifier *pm)
{
   png_byte colour_type = 0;
   png_byte bit_depth = 0;
   unsigned int palette_number = 0;
   while (next_format(&colour_type, &bit_depth, &palette_number, 0))
   {
      png_uint_32 counter = 0;
      size_t base_pos;
      char name[64];
      base_pos = safecat(name, sizeof name, 0, "transform:");
      for (;;)
      {
         size_t pos = base_pos;
         PNG_CONST image_transform *list = 0;
         


         counter = image_transform_add(&list, 1, counter,
            name, sizeof name, &pos, colour_type, bit_depth);
         if (counter == 0)
            break;
         
         do
         {
            pm->repeat = 0;
            transform_test(pm, FILEID(colour_type, bit_depth, palette_number,
               pm->interlace_type, 0, 0, 0), list, name);
            if (fail(pm))
               return;
         }
         while (pm->repeat);
      }
   }
}

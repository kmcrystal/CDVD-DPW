static void
generate_row(png_bytep row, size_t rowbytes, unsigned int y, int color_type,
   int bit_depth, png_const_bytep gamma_table, double conv,
   unsigned int *colors)
{
   png_uint_32 size_max = image_size_of_type(color_type, bit_depth, colors)-1;
   png_uint_32 depth_max = (1U << bit_depth)-1; 
   if (colors[0] == 0) switch (channels_of_type(color_type))
   {
   


      case 1:
         {
            png_uint_32 x;
            png_uint_32 base = 2*size_max - abs(2*y-size_max);
            for (x=0; x<=size_max; ++x)
            {
               png_uint_32 luma = base - abs(2*x-size_max);
               


               luma = (luma*depth_max + size_max) / (2*size_max);
               set_value(row, rowbytes, x, bit_depth, luma, gamma_table, conv);
            }
         }
         break;
   


      case 2:
         {
            png_uint_32 alpha = (depth_max * y * 2 + size_max) / (2 * size_max);
            png_uint_32 x;
            for (x=0; x<=size_max; ++x)
            {
               set_value(row, rowbytes, 2*x, bit_depth,
                  (depth_max * x * 2 + size_max) / (2 * size_max), gamma_table,
                  conv);
               set_value(row, rowbytes, 2*x+1, bit_depth, alpha, gamma_table,
                  conv);
            }
         }
         break;
   


      case 3:
         {
            




            png_uint_32 Y = (depth_max * y * 2 + size_max) / (2 * size_max);
            png_uint_32 x;
            






            for (x=0; x<=size_max; ++x)
            {
               set_value(row, rowbytes, 3*x+0, bit_depth,  Y,
                     gamma_table, conv);
               set_value(row, rowbytes, 3*x+1, bit_depth, 
                  (depth_max * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
               set_value(row, rowbytes, 3*x+2, bit_depth, 
                  (Y * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
            }
         }
         break;
   


      case 4:
         {
            





            png_uint_32 Y = (depth_max * y * 2 + size_max) / (2 * size_max);
            png_uint_32 x;
            







            for (x=0; x<=size_max; ++x)
            {
               set_value(row, rowbytes, 4*x+0, bit_depth, 
                  ((depth_max-Y) * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
               set_value(row, rowbytes, 4*x+1, bit_depth, 
                  (Y * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
               set_value(row, rowbytes, 4*x+2, bit_depth, 
                  Y - (Y * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
               set_value(row, rowbytes, 4*x+3, bit_depth, 
                  Y + ((depth_max-Y) * x * 2 + size_max) / (2 * size_max),
                  gamma_table, conv);
            }
         }
         break;
      default:
         fprintf(stderr, "makepng: internal bad channel count\n");
         exit(2);
   }
   else if (color_type & PNG_COLOR_MASK_PALETTE)
   {
      


      memset(row, 0, rowbytes);
   }
   else if (colors[0] == channels_of_type(color_type))
      switch (channels_of_type(color_type))
      {
         case 1:
            {
               const png_uint_32 luma = colors[1];
               png_uint_32 x;
               for (x=0; x<=size_max; ++x)
                  set_value(row, rowbytes, x, bit_depth, luma, gamma_table,
                     conv);
            }
            break;
         case 2:
            {
               const png_uint_32 luma = colors[1];
               const png_uint_32 alpha = colors[2];
               png_uint_32 x;
               for (x=0; x<size_max; ++x)
               {
                  set_value(row, rowbytes, 2*x, bit_depth, luma, gamma_table,
                     conv);
                  set_value(row, rowbytes, 2*x+1, bit_depth, alpha, gamma_table,
                     conv);
               }
            }
            break;
         case 3:
            {
               const png_uint_32 red = colors[1];
               const png_uint_32 green = colors[2];
               const png_uint_32 blue = colors[3];
               png_uint_32 x;
               for (x=0; x<=size_max; ++x)
               {
                  set_value(row, rowbytes, 3*x+0, bit_depth, red, gamma_table,
                     conv);
                  set_value(row, rowbytes, 3*x+1, bit_depth, green, gamma_table,
                     conv);
                  set_value(row, rowbytes, 3*x+2, bit_depth, blue, gamma_table,
                     conv);
               }
            }
            break;
         case 4:
            {
               const png_uint_32 red = colors[1];
               const png_uint_32 green = colors[2];
               const png_uint_32 blue = colors[3];
               const png_uint_32 alpha = colors[4];
               png_uint_32 x;
               for (x=0; x<=size_max; ++x)
               {
                  set_value(row, rowbytes, 4*x+0, bit_depth, red, gamma_table,
                     conv);
                  set_value(row, rowbytes, 4*x+1, bit_depth, green, gamma_table,
                     conv);
                  set_value(row, rowbytes, 4*x+2, bit_depth, blue, gamma_table,
                     conv);
                  set_value(row, rowbytes, 4*x+3, bit_depth, alpha, gamma_table,
                     conv);
               }
            }
         break;
         default:
            fprintf(stderr, "makepng: internal bad channel count\n");
            exit(2);
      }
   else
   {
      fprintf(stderr,
         "makepng: --color: count(%u) does not match channels(%u)\n",
         colors[0], channels_of_type(color_type));
      exit(1);
   }
}

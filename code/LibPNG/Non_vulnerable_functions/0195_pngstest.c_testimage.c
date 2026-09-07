static int
testimage(Image *image, png_uint_32 opts, format_list *pf)
{
   int result;
   Image copy;
   
   checkopaque(image);
   copy = *image;
   copy.opts = opts;
   copy.buffer = NULL;
   copy.bufsize = 0;
   copy.allocsize = 0;
   image->input_file = NULL;
   image->input_memory = NULL;
   image->input_memory_size = 0;
   image->tmpfile_name[0] = 0;
   {
      png_uint_32 counter;
      Image output;
      newimage(&output);
      result = 1;
      



      for (counter=0; counter<2*FORMAT_COUNT; ++counter)
         if (format_isset(pf, counter >> 1))
      {
         png_uint_32 format = counter >> 1;
         png_color background_color;
         png_colorp background = NULL;
         





         if ((counter & 1) == 0)
         {
            if ((format & PNG_FORMAT_FLAG_ALPHA) == 0 &&
               (image->image.format & PNG_FORMAT_FLAG_ALPHA) != 0)
            {
               


               random_color(&background_color);
               background = &background_color;
               



               if ((format & PNG_FORMAT_FLAG_COLORMAP) != 0 &&
                  (format & PNG_FORMAT_FLAG_LINEAR) == 0)
                  ++counter;
            }
            


            else
               ++counter;
         }
         
         resetimage(&copy);
         copy.opts = opts; 
         result = read_file(&copy, format, background);
         if (!result)
            break;
         
         result = compare_two_images(image, &copy, 0, background);
         if (!result)
            break;
#        ifdef PNG_SIMPLIFIED_WRITE_SUPPORTED
            



            output.opts = opts;
            result = write_one_file(&output, &copy, 0);
            if (!result)
               break;
            


            result = compare_two_images(image, &output, 0, background);
            if (!result)
               break;
            if ((format & PNG_FORMAT_FLAG_LINEAR) != 0 &&
               (format & PNG_FORMAT_FLAG_COLORMAP) == 0)
            {
               

               output.opts = opts;
               result = write_one_file(&output, &copy, 1);
               if (!result)
                  break;
               








               result = compare_two_images(image, &output, 1,
                  background);
               if (!result)
                  break;
            }
#        endif 
      }
      freeimage(&output);
   }
   freeimage(&copy);
   return result;
}

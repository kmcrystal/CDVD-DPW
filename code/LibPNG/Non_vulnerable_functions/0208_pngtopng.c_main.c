#include "../../png.h"
int main(int argc, const char **argv)
{
   int result = 1;
   if (argc == 3)
   {
      png_image image;
      
      memset(&image, 0, sizeof image);
      image.version = PNG_IMAGE_VERSION;
      if (png_image_begin_read_from_file(&image, argv[1]))
      {
         png_bytep buffer;
         


         image.format = PNG_FORMAT_RGBA;
         buffer = malloc(PNG_IMAGE_SIZE(image));
         if (buffer != NULL)
         {
            if (png_image_finish_read(&image, NULL, buffer,
               0, NULL))
            {
               if (png_image_write_to_file(&image, argv[2],
                  0, buffer, 0,
                  NULL))
                  result = 0;
               else
                  fprintf(stderr, "pngtopng: write %s: %s\n", argv[2],
                      image.message);
               free(buffer);
            }
            else
            {
               fprintf(stderr, "pngtopng: read %s: %s\n", argv[1],
                   image.message);
               



               png_image_free(&image);
            }
         }
         else
            fprintf(stderr, "pngtopng: out of memory: %lu bytes\n",
               (unsigned long)PNG_IMAGE_SIZE(image));
      }
      else
         
         fprintf(stderr, "pngtopng: %s: %s\n", argv[1], image.message);
   }
   else
      
      fprintf(stderr, "pngtopng: usage: pngtopng input-file output-file\n");
   return result;
}

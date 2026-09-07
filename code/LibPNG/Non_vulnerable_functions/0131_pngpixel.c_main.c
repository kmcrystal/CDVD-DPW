}
int main(int argc, const char **argv)
{
   





   volatile int result = 1;
   if (argc == 4)
   {
      long x = atol(argv[1]);
      long y = atol(argv[2]);
      FILE *f = fopen(argv[3], "rb");
      volatile png_bytep row = NULL;
      if (f != NULL)
      {
         





         png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
         if (png_ptr != NULL)
         {
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr != NULL)
            {
               


               
               if (setjmp(png_jmpbuf(png_ptr)) == 0)
               {
                  png_uint_32 width, height;
                  int bit_depth, color_type, interlace_method,
                     compression_method, filter_method;
                  png_bytep row_tmp;
                  





                  png_init_io(png_ptr, f);
                  


                  png_read_info(png_ptr, info_ptr);
                  




                  row = png_malloc(png_ptr, png_get_rowbytes(png_ptr,
                     info_ptr));
                  


                  row_tmp = row;
                  



                  if (png_get_IHDR(png_ptr, info_ptr, &width, &height,
                     &bit_depth, &color_type, &interlace_method,
                     &compression_method, &filter_method))
                  {
                     int passes, pass;
                     




                     switch (interlace_method)
                     {
                        case PNG_INTERLACE_NONE:
                           passes = 1;
                           break;
                        case PNG_INTERLACE_ADAM7:
                           passes = PNG_INTERLACE_ADAM7_PASSES;
                           break;
                        default:
                           png_error(png_ptr, "pngpixel: unknown interlace");
                     }
                     
                     png_start_read_image(png_ptr);
                     for (pass=0; pass<passes; ++pass)
                     {
                        png_uint_32 ystart, xstart, ystep, xstep;
                        png_uint_32 py;
                        if (interlace_method == PNG_INTERLACE_ADAM7)
                        {
                           






                           if (PNG_PASS_COLS(width, pass) == 0)
                              continue;
                           



                           xstart = PNG_PASS_START_COL(pass);
                           ystart = PNG_PASS_START_ROW(pass);
                           xstep = PNG_PASS_COL_OFFSET(pass);
                           ystep = PNG_PASS_ROW_OFFSET(pass);
                        }
                        else
                        {
                           ystart = xstart = 0;
                           ystep = xstep = 1;
                        }
                        



                        for (py = ystart; py < height; py += ystep)
                        {
                           png_uint_32 px, ppx;
                           









                           png_read_row(png_ptr, row_tmp, NULL);
                           



                           if (y == py) for (px = xstart, ppx = 0;
                              px < width; px += xstep, ++ppx) if (x == px)
                           {
                              


                              print_pixel(png_ptr, info_ptr, row_tmp, ppx);
                              


                              goto pass_loop_end;
                           } 
                        } 
                     } 
                     
                  pass_loop_end:
                     row = NULL;
                     png_free(png_ptr, row_tmp);
                  }
                  else
                     png_error(png_ptr, "pngpixel: png_get_IHDR failed");
               }
               else
               {
                  



                  if (row != NULL)
                  {
                     




                     png_bytep row_tmp = row;
                     row = NULL;
                     png_free(png_ptr, row_tmp);
                  }
               }
               png_destroy_info_struct(png_ptr, &info_ptr);
            }
            else
               fprintf(stderr, "pngpixel: out of memory allocating png_info\n");
            png_destroy_read_struct(&png_ptr, NULL, NULL);
         }
         else
            fprintf(stderr, "pngpixel: out of memory allocating png_struct\n");
      }
      else
         fprintf(stderr, "pngpixel: %s: could not open file\n", argv[3]);
   }
   else
      
      fprintf(stderr, "pngpixel: usage: pngpixel x y png-file\n");
   return result;
}

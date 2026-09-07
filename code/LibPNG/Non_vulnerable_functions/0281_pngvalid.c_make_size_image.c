static void
make_size_image(png_store* PNG_CONST ps, png_byte PNG_CONST colour_type,
    png_byte PNG_CONST bit_depth, int PNG_CONST interlace_type,
    png_uint_32 PNG_CONST w, png_uint_32 PNG_CONST h,
    int PNG_CONST do_interlace)
{
   context(ps, fault);
   



   check_interlace_type(interlace_type);
   Try
   {
      png_infop pi;
      png_structp pp;
      unsigned int pixel_size;
      
      char name[FILE_NAME_SIZE];
      PNG_CONST png_uint_32 id = FILEID(colour_type, bit_depth, 0,
         interlace_type, w, h, do_interlace);
      standard_name_from_id(name, sizeof name, 0, id);
      pp = set_store_for_write(ps, &pi, name);
      



      if (pp == NULL)
         Throw ps;
      png_set_IHDR(pp, pi, w, h, bit_depth, colour_type, interlace_type,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
#ifdef PNG_TEXT_SUPPORTED
      {
         static char key[] = "image name"; 
         size_t pos;
         png_text text;
         char copy[FILE_NAME_SIZE];
         


         text.compression = TEXT_COMPRESSION;
         text.key = key;
         
         pos = safecat(copy, sizeof copy, 0, ps->wname);
         text.text = copy;
         text.text_length = pos;
         text.itxt_length = 0;
         text.lang = 0;
         text.lang_key = 0;
         png_set_text(pp, pi, &text, 1);
      }
#endif
      if (colour_type == 3) 
         init_standard_palette(ps, pp, pi, 1U << bit_depth, 0);
      png_write_info(pp, pi);
      



      pixel_size = bit_size(pp, colour_type, bit_depth);
      if (png_get_rowbytes(pp, pi) != ((w * pixel_size) + 7) / 8)
         png_error(pp, "row size incorrect");
      else
      {
         int npasses = npasses_from_interlace_type(pp, interlace_type);
         png_uint_32 y;
         int pass;
#        ifdef PNG_WRITE_FILTER_SUPPORTED
            int nfilter = PNG_FILTER_VALUE_LAST;
#        endif
         png_byte image[16][SIZE_ROWMAX];
         


         memset(image, 0xff, sizeof image);
         if (!do_interlace && npasses != png_set_interlace_handling(pp))
            png_error(pp, "write: png_set_interlace_handling failed");
         
         for (y=0; y<h; ++y)
            size_row(image[y], w * pixel_size, y);
         for (pass=0; pass<npasses; ++pass)
         {
            
            PNG_CONST png_uint_32 wPass = PNG_PASS_COLS(w, pass);
            



            for (y=0; y<h; ++y)
            {
               png_const_bytep row = image[y];
               png_byte tempRow[SIZE_ROWMAX];
               



               if (do_interlace && interlace_type == PNG_INTERLACE_ADAM7)
               {
                  





                  if (PNG_ROW_IN_INTERLACE_PASS(y, pass) && wPass > 0)
                  {
                     


                     memset(tempRow, 0xff, sizeof tempRow);
                     interlace_row(tempRow, row, pixel_size, w, pass);
                     row = tempRow;
                  }
                  else
                     continue;
               }
#           ifdef PNG_WRITE_FILTER_SUPPORTED
               









               png_set_filter(pp, 0,
                  nfilter >= PNG_FILTER_VALUE_LAST ? PNG_ALL_FILTERS : nfilter);
               if (nfilter-- == 0)
                  nfilter = PNG_FILTER_VALUE_LAST-1;
#           endif
               png_write_row(pp, row);
            }
         }
      }
#ifdef PNG_TEXT_SUPPORTED
      {
         static char key[] = "end marker";
         static char comment[] = "end";
         png_text text;
         


         text.compression = TEXT_COMPRESSION;
         text.key = key;
         text.text = comment;
         text.text_length = (sizeof comment)-1;
         text.itxt_length = 0;
         text.lang = 0;
         text.lang_key = 0;
         png_set_text(pp, pi, &text, 1);
      }
#endif
      png_write_end(pp, pi);
      
      store_storefile(ps, id);
      store_write_reset(ps);
   }
   Catch(fault)
   {
      



      store_write_reset(fault);
   }
}

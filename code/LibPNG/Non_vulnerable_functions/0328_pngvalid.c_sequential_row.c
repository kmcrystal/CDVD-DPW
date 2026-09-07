static void
sequential_row(standard_display *dp, png_structp pp, png_infop pi,
    PNG_CONST int iImage, PNG_CONST int iDisplay)
{
   PNG_CONST int         npasses = dp->npasses;
   PNG_CONST int         do_interlace = dp->do_interlace &&
      dp->interlace_type == PNG_INTERLACE_ADAM7;
   PNG_CONST png_uint_32 height = standard_height(pp, dp->id);
   PNG_CONST png_uint_32 width = standard_width(pp, dp->id);
   PNG_CONST png_store*  ps = dp->ps;
   int pass;
   for (pass=0; pass<npasses; ++pass)
   {
      png_uint_32 y;
      png_uint_32 wPass = PNG_PASS_COLS(width, pass);
      for (y=0; y<height; ++y)
      {
         if (do_interlace)
         {
            


            if (wPass > 0 && PNG_ROW_IN_INTERLACE_PASS(y, pass))
            {
               


               png_byte row[STANDARD_ROWMAX], display[STANDARD_ROWMAX];
               





               memset(row, 0xc5, sizeof row);
               memset(display, 0x5c, sizeof display);
               png_read_row(pp, row, display);
               if (iImage >= 0)
                  deinterlace_row(store_image_row(ps, pp, iImage, y), row,
                     dp->pixel_size, dp->w, pass);
               if (iDisplay >= 0)
                  deinterlace_row(store_image_row(ps, pp, iDisplay, y), display,
                     dp->pixel_size, dp->w, pass);
            }
         }
         else
            png_read_row(pp,
               iImage >= 0 ? store_image_row(ps, pp, iImage, y) : NULL,
               iDisplay >= 0 ? store_image_row(ps, pp, iDisplay, y) : NULL);
      }
   }
   


   png_read_end(pp, pi);
}

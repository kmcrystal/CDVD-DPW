int
test_one_file(PNG_CONST char *inname, PNG_CONST char *outname)
{
   static png_FILE_p fpin;
   static png_FILE_p fpout;  
   png_structp read_ptr;
   png_infop read_info_ptr, end_info_ptr;
#ifdef PNG_WRITE_SUPPORTED
   png_structp write_ptr;
   png_infop write_info_ptr;
   png_infop write_end_info_ptr;
#else
   png_structp write_ptr = NULL;
   png_infop write_info_ptr = NULL;
   png_infop write_end_info_ptr = NULL;
#endif
   png_bytep row_buf;
   png_uint_32 y;
   png_uint_32 width, height;
   int num_pass, pass;
   int bit_depth, color_type;
#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   jmp_buf png_jmpbuf;
#endif
#endif
   char inbuf[256], outbuf[256];
   row_buf = NULL;
   if ((fpin = fopen(inname, "rb")) == NULL)
   {
      fprintf(STDERR, "Could not find input file %s\n", inname);
      return (1);
   }
   if ((fpout = fopen(outname, "wb")) == NULL)
   {
      fprintf(STDERR, "Could not open output file %s\n", outname);
      FCLOSE(fpin);
      return (1);
   }
   pngtest_debug("Allocating read and write structures");
#if defined(PNG_USER_MEM_SUPPORTED) && PNG_DEBUG
   read_ptr =
      png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL,
      NULL, NULL, NULL, png_debug_malloc, png_debug_free);
#else
   read_ptr =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
#endif
#ifndef PNG_STDIO_SUPPORTED
   png_set_error_fn(read_ptr, (png_voidp)inname, pngtest_error,
       pngtest_warning);
#endif
#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
   user_chunk_data[0] = 0;
   user_chunk_data[1] = 0;
   user_chunk_data[2] = 0;
   user_chunk_data[3] = 0;
   png_set_read_user_chunk_fn(read_ptr, user_chunk_data,
     read_user_chunk_callback);
#endif
#ifdef PNG_WRITE_SUPPORTED
#if defined(PNG_USER_MEM_SUPPORTED) && PNG_DEBUG
   write_ptr =
      png_create_write_struct_2(PNG_LIBPNG_VER_STRING, NULL,
      NULL, NULL, NULL, png_debug_malloc, png_debug_free);
#else
   write_ptr =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
#endif
#ifndef PNG_STDIO_SUPPORTED
   png_set_error_fn(write_ptr, (png_voidp)inname, pngtest_error,
       pngtest_warning);
#endif
#endif
   pngtest_debug("Allocating read_info, write_info and end_info structures");
   read_info_ptr = png_create_info_struct(read_ptr);
   end_info_ptr = png_create_info_struct(read_ptr);
#ifdef PNG_WRITE_SUPPORTED
   write_info_ptr = png_create_info_struct(write_ptr);
   write_end_info_ptr = png_create_info_struct(write_ptr);
#endif
#ifdef PNG_SETJMP_SUPPORTED
   pngtest_debug("Setting jmpbuf for read struct");
#ifdef USE_FAR_KEYWORD
   if (setjmp(png_jmpbuf))
#else
   if (setjmp(png_jmpbuf(read_ptr)))
#endif
   {
      fprintf(STDERR, "%s -> %s: libpng read error\n", inname, outname);
      png_free(read_ptr, row_buf);
      row_buf = NULL;
      png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
#ifdef PNG_WRITE_SUPPORTED
      png_destroy_info_struct(write_ptr, &write_end_info_ptr);
      png_destroy_write_struct(&write_ptr, &write_info_ptr);
#endif
      FCLOSE(fpin);
      FCLOSE(fpout);
      return (1);
   }
#ifdef USE_FAR_KEYWORD
   png_memcpy(png_jmpbuf(read_ptr), png_jmpbuf, png_sizeof(jmp_buf));
#endif
#ifdef PNG_WRITE_SUPPORTED
   pngtest_debug("Setting jmpbuf for write struct");
#ifdef USE_FAR_KEYWORD
   if (setjmp(png_jmpbuf))
#else
   if (setjmp(png_jmpbuf(write_ptr)))
#endif
   {
      fprintf(STDERR, "%s -> %s: libpng write error\n", inname, outname);
      png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
      png_destroy_info_struct(write_ptr, &write_end_info_ptr);
#ifdef PNG_WRITE_SUPPORTED
      png_destroy_write_struct(&write_ptr, &write_info_ptr);
#endif
      FCLOSE(fpin);
      FCLOSE(fpout);
      return (1);
   }
#ifdef USE_FAR_KEYWORD
   png_memcpy(png_jmpbuf(write_ptr), png_jmpbuf, png_sizeof(jmp_buf));
#endif
#endif
#endif
   pngtest_debug("Initializing input and output streams");
#ifdef PNG_STDIO_SUPPORTED
   png_init_io(read_ptr, fpin);
#  ifdef PNG_WRITE_SUPPORTED
   png_init_io(write_ptr, fpout);
#  endif
#else
   png_set_read_fn(read_ptr, (png_voidp)fpin, pngtest_read_data);
#  ifdef PNG_WRITE_SUPPORTED
   png_set_write_fn(write_ptr, (png_voidp)fpout,  pngtest_write_data,
#    ifdef PNG_WRITE_FLUSH_SUPPORTED
      pngtest_flush);
#    else
      NULL);
#    endif
#  endif
#endif
   if (status_dots_requested == 1)
   {
#ifdef PNG_WRITE_SUPPORTED
      png_set_write_status_fn(write_ptr, write_row_callback);
#endif
      png_set_read_status_fn(read_ptr, read_row_callback);
   }
   else
   {
#ifdef PNG_WRITE_SUPPORTED
      png_set_write_status_fn(write_ptr, NULL);
#endif
      png_set_read_status_fn(read_ptr, NULL);
   }
#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   {
      int i;
      for (i = 0; i<256; i++)
         filters_used[i] = 0;
      png_set_read_user_transform_fn(read_ptr, count_filters);
   }
#endif
#ifdef PNG_WRITE_USER_TRANSFORM_SUPPORTED
   zero_samples = 0;
   png_set_write_user_transform_fn(write_ptr, count_zero_samples);
#endif
#ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
#  ifndef PNG_HANDLE_CHUNK_ALWAYS
#    define PNG_HANDLE_CHUNK_ALWAYS       3
#  endif
   png_set_keep_unknown_chunks(read_ptr, PNG_HANDLE_CHUNK_ALWAYS,
      NULL, 0);
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
#  ifndef PNG_HANDLE_CHUNK_IF_SAFE
#    define PNG_HANDLE_CHUNK_IF_SAFE      2
#  endif
   png_set_keep_unknown_chunks(write_ptr, PNG_HANDLE_CHUNK_IF_SAFE,
      NULL, 0);
#endif
   pngtest_debug("Reading info struct");
   png_read_info(read_ptr, read_info_ptr);
   pngtest_debug("Transferring info struct");
   {
      int interlace_type, compression_type, filter_type;
      if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
          &color_type, &interlace_type, &compression_type, &filter_type))
      {
         png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
            color_type, interlace_type, compression_type, filter_type);
#else
            color_type, PNG_INTERLACE_NONE, compression_type, filter_type);
#endif
      }
   }
#ifdef PNG_FIXED_POINT_SUPPORTED
#ifdef PNG_cHRM_SUPPORTED
   {
      png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x,
         blue_y;
      if (png_get_cHRM_fixed(read_ptr, read_info_ptr, &white_x, &white_y,
         &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y))
      {
         png_set_cHRM_fixed(write_ptr, write_info_ptr, white_x, white_y, red_x,
            red_y, green_x, green_y, blue_x, blue_y);
      }
   }
#endif
#ifdef PNG_gAMA_SUPPORTED
   {
      png_fixed_point gamma;
      if (png_get_gAMA_fixed(read_ptr, read_info_ptr, &gamma))
         png_set_gAMA_fixed(write_ptr, write_info_ptr, gamma);
   }
#endif
#else 
#ifdef PNG_FLOATING_POINT_SUPPORTED
#ifdef PNG_cHRM_SUPPORTED
   {
      double white_x, white_y, red_x, red_y, green_x, green_y, blue_x,
         blue_y;
      if (png_get_cHRM(read_ptr, read_info_ptr, &white_x, &white_y, &red_x,
         &red_y, &green_x, &green_y, &blue_x, &blue_y))
      {
         png_set_cHRM(write_ptr, write_info_ptr, white_x, white_y, red_x,
            red_y, green_x, green_y, blue_x, blue_y);
      }
   }
#endif
#ifdef PNG_gAMA_SUPPORTED
   {
      double gamma;
      if (png_get_gAMA(read_ptr, read_info_ptr, &gamma))
         png_set_gAMA(write_ptr, write_info_ptr, gamma);
   }
#endif
#endif 
#endif 
#ifdef PNG_iCCP_SUPPORTED
   {
      png_charp name;
      png_bytep profile;
      png_uint_32 proflen;
      int compression_type;
      if (png_get_iCCP(read_ptr, read_info_ptr, &name, &compression_type,
                      &profile, &proflen))
      {
         png_set_iCCP(write_ptr, write_info_ptr, name, compression_type,
                      profile, proflen);
      }
   }
#endif
#ifdef PNG_sRGB_SUPPORTED
   {
      int intent;
      if (png_get_sRGB(read_ptr, read_info_ptr, &intent))
         png_set_sRGB(write_ptr, write_info_ptr, intent);
   }
#endif
   {
      png_colorp palette;
      int num_palette;
      if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette))
         png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
   }
#ifdef PNG_bKGD_SUPPORTED
   {
      png_color_16p background;
      if (png_get_bKGD(read_ptr, read_info_ptr, &background))
      {
         png_set_bKGD(write_ptr, write_info_ptr, background);
      }
   }
#endif
#ifdef PNG_hIST_SUPPORTED
   {
      png_uint_16p hist;
      if (png_get_hIST(read_ptr, read_info_ptr, &hist))
         png_set_hIST(write_ptr, write_info_ptr, hist);
   }
#endif
#ifdef PNG_oFFs_SUPPORTED
   {
      png_int_32 offset_x, offset_y;
      int unit_type;
      if (png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y,
          &unit_type))
      {
         png_set_oFFs(write_ptr, write_info_ptr, offset_x, offset_y, unit_type);
      }
   }
#endif
#ifdef PNG_pCAL_SUPPORTED
   {
      png_charp purpose, units;
      png_charpp params;
      png_int_32 X0, X1;
      int type, nparams;
      if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type,
         &nparams, &units, &params))
      {
         png_set_pCAL(write_ptr, write_info_ptr, purpose, X0, X1, type,
            nparams, units, params);
      }
   }
#endif
#ifdef PNG_pHYs_SUPPORTED
   {
      png_uint_32 res_x, res_y;
      int unit_type;
      if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y, &unit_type))
         png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);
   }
#endif
#ifdef PNG_sBIT_SUPPORTED
   {
      png_color_8p sig_bit;
      if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit))
         png_set_sBIT(write_ptr, write_info_ptr, sig_bit);
   }
#endif
#ifdef PNG_sCAL_SUPPORTED
#ifdef PNG_FLOATING_POINT_SUPPORTED
   {
      int unit;
      double scal_width, scal_height;
      if (png_get_sCAL(read_ptr, read_info_ptr, &unit, &scal_width,
         &scal_height))
      {
         png_set_sCAL(write_ptr, write_info_ptr, unit, scal_width, scal_height);
      }
   }
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
   {
      int unit;
      png_charp scal_width, scal_height;
      if (png_get_sCAL_s(read_ptr, read_info_ptr, &unit, &scal_width,
          &scal_height))
      {
         png_set_sCAL_s(write_ptr, write_info_ptr, unit, scal_width,
             scal_height);
      }
   }
#endif
#endif
#endif
#ifdef PNG_TEXT_SUPPORTED
   {
      png_textp text_ptr;
      int num_text;
      if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
      {
         pngtest_debug1("Handling %d iTXt/tEXt/zTXt chunks", num_text);
         png_set_text(write_ptr, write_info_ptr, text_ptr, num_text);
      }
   }
#endif
#ifdef PNG_tIME_SUPPORTED
   {
      png_timep mod_time;
      if (png_get_tIME(read_ptr, read_info_ptr, &mod_time))
      {
         png_set_tIME(write_ptr, write_info_ptr, mod_time);
#ifdef PNG_TIME_RFC1123_SUPPORTED
         



         png_memcpy(tIME_string,
                    png_convert_to_rfc1123(read_ptr, mod_time),
                    png_sizeof(tIME_string));
         tIME_string[png_sizeof(tIME_string) - 1] = '\0';
         tIME_chunk_present++;
#endif 
      }
   }
#endif
#ifdef PNG_tRNS_SUPPORTED
   {
      png_bytep trans_alpha;
      int num_trans;
      png_color_16p trans_color;
      if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans,
         &trans_color))
      {
         int sample_max = (1 << bit_depth);
         
         if (!((color_type == PNG_COLOR_TYPE_GRAY &&
             (int)trans_color->gray > sample_max) ||
             (color_type == PNG_COLOR_TYPE_RGB &&
             ((int)trans_color->red > sample_max ||
             (int)trans_color->green > sample_max ||
             (int)trans_color->blue > sample_max))))
            png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans,
               trans_color);
      }
   }
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   {
      png_unknown_chunkp unknowns;
      int num_unknowns = png_get_unknown_chunks(read_ptr, read_info_ptr,
         &unknowns);
      if (num_unknowns)
      {
         int i;
         png_set_unknown_chunks(write_ptr, write_info_ptr, unknowns,
           num_unknowns);
         



         for (i = 0; i < num_unknowns; i++)
           png_set_unknown_chunk_location(write_ptr, write_info_ptr, i,
             unknowns[i].location);
      }
   }
#endif
#ifdef PNG_WRITE_SUPPORTED
   pngtest_debug("Writing info struct");



   png_write_info(write_ptr, write_info_ptr);
#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
   if (user_chunk_data[0] != 0)
   {
      png_byte png_sTER[5] = {115,  84,  69,  82, '\0'};
      unsigned char
        ster_chunk_data[1];
      if (verbose)
         fprintf(STDERR, "\n stereo mode = %lu\n",
           (unsigned long)(user_chunk_data[0] - 1));
      ster_chunk_data[0]=(unsigned char)(user_chunk_data[0] - 1);
      png_write_chunk(write_ptr, png_sTER, ster_chunk_data, 1);
   }
   if (user_chunk_data[1] != 0 || user_chunk_data[2] != 0)
   {
      png_byte png_vpAg[5] = {118, 112,  65, 103, '\0'};
      unsigned char
        vpag_chunk_data[9];
      if (verbose)
         fprintf(STDERR, " vpAg = %lu x %lu, units = %lu\n",
           (unsigned long)user_chunk_data[1],
           (unsigned long)user_chunk_data[2],
           (unsigned long)user_chunk_data[3]);
      png_save_uint_32(vpag_chunk_data, user_chunk_data[1]);
      png_save_uint_32(vpag_chunk_data + 4, user_chunk_data[2]);
      vpag_chunk_data[8] = (unsigned char)(user_chunk_data[3] & 0xff);
      png_write_chunk(write_ptr, png_vpAg, vpag_chunk_data, 9);
   }
#endif
#endif
#ifdef SINGLE_ROWBUF_ALLOC
   pngtest_debug("Allocating row buffer...");
   row_buf = (png_bytep)png_malloc(read_ptr,
      png_get_rowbytes(read_ptr, read_info_ptr));
   pngtest_debug1("\t0x%08lx", (unsigned long)row_buf);
#endif 
   pngtest_debug("Writing row data");
#if defined(PNG_READ_INTERLACING_SUPPORTED) || \
  defined(PNG_WRITE_INTERLACING_SUPPORTED)
   num_pass = png_set_interlace_handling(read_ptr);
#  ifdef PNG_WRITE_SUPPORTED
   png_set_interlace_handling(write_ptr);
#  endif
#else
   num_pass = 1;
#endif
#ifdef PNGTEST_TIMING
   t_stop = (float)clock();
   t_misc += (t_stop - t_start);
   t_start = t_stop;
#endif
   for (pass = 0; pass < num_pass; pass++)
   {
      pngtest_debug1("Writing row data for pass %d", pass);
      for (y = 0; y < height; y++)
      {
#ifndef SINGLE_ROWBUF_ALLOC
         pngtest_debug2("Allocating row buffer (pass %d, y = %u)...", pass, y);
         row_buf = (png_bytep)png_malloc(read_ptr,
            png_get_rowbytes(read_ptr, read_info_ptr));
         pngtest_debug2("\t0x%08lx (%u bytes)", (unsigned long)row_buf,
            png_get_rowbytes(read_ptr, read_info_ptr));
#endif 
         png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);
#ifdef PNG_WRITE_SUPPORTED
#ifdef PNGTEST_TIMING
         t_stop = (float)clock();
         t_decode += (t_stop - t_start);
         t_start = t_stop;
#endif
         png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);
#ifdef PNGTEST_TIMING
         t_stop = (float)clock();
         t_encode += (t_stop - t_start);
         t_start = t_stop;
#endif
#endif 
#ifndef SINGLE_ROWBUF_ALLOC
         pngtest_debug2("Freeing row buffer (pass %d, y = %u)", pass, y);
         png_free(read_ptr, row_buf);
         row_buf = NULL;
#endif 
      }
   }
#ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
   png_free_data(read_ptr, read_info_ptr, PNG_FREE_UNKN, -1);
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   png_free_data(write_ptr, write_info_ptr, PNG_FREE_UNKN, -1);
#endif
   pngtest_debug("Reading and writing end_info data");
   png_read_end(read_ptr, end_info_ptr);
#ifdef PNG_TEXT_SUPPORTED
   {
      png_textp text_ptr;
      int num_text;
      if (png_get_text(read_ptr, end_info_ptr, &text_ptr, &num_text) > 0)
      {
         pngtest_debug1("Handling %d iTXt/tEXt/zTXt chunks", num_text);
         png_set_text(write_ptr, write_end_info_ptr, text_ptr, num_text);
      }
   }
#endif
#ifdef PNG_tIME_SUPPORTED
   {
      png_timep mod_time;
      if (png_get_tIME(read_ptr, end_info_ptr, &mod_time))
      {
         png_set_tIME(write_ptr, write_end_info_ptr, mod_time);
#ifdef PNG_TIME_RFC1123_SUPPORTED
         


         png_memcpy(tIME_string,
                    png_convert_to_rfc1123(read_ptr, mod_time),
                    png_sizeof(tIME_string));
         tIME_string[png_sizeof(tIME_string) - 1] = '\0';
         tIME_chunk_present++;
#endif 
      }
   }
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   {
      png_unknown_chunkp unknowns;
      int num_unknowns = png_get_unknown_chunks(read_ptr, end_info_ptr,
         &unknowns);
      if (num_unknowns)
      {
         int i;
         png_set_unknown_chunks(write_ptr, write_end_info_ptr, unknowns,
           num_unknowns);
         



         for (i = 0; i < num_unknowns; i++)
           png_set_unknown_chunk_location(write_ptr, write_end_info_ptr, i,
             unknowns[i].location);
      }
   }
#endif
#ifdef PNG_WRITE_SUPPORTED
   png_write_end(write_ptr, write_end_info_ptr);
#endif
#ifdef PNG_EASY_ACCESS_SUPPORTED
   if (verbose)
   {
      png_uint_32 iwidth, iheight;
      iwidth = png_get_image_width(write_ptr, write_info_ptr);
      iheight = png_get_image_height(write_ptr, write_info_ptr);
      fprintf(STDERR, "\n Image width = %lu, height = %lu\n",
         (unsigned long)iwidth, (unsigned long)iheight);
   }
#endif
   pngtest_debug("Destroying data structs");
#ifdef SINGLE_ROWBUF_ALLOC
   pngtest_debug("destroying row_buf for read_ptr");
   png_free(read_ptr, row_buf);
   row_buf = NULL;
#endif 
   pngtest_debug("destroying read_ptr, read_info_ptr, end_info_ptr");
   png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
#ifdef PNG_WRITE_SUPPORTED
   pngtest_debug("destroying write_end_info_ptr");
   png_destroy_info_struct(write_ptr, &write_end_info_ptr);
   pngtest_debug("destroying write_ptr, write_info_ptr");
   png_destroy_write_struct(&write_ptr, &write_info_ptr);
#endif
   pngtest_debug("Destruction complete.");
   FCLOSE(fpin);
   FCLOSE(fpout);
   pngtest_debug("Opening files for comparison");
   if ((fpin = fopen(inname, "rb")) == NULL)
   {
      fprintf(STDERR, "Could not find file %s\n", inname);
      return (1);
   }
   if ((fpout = fopen(outname, "rb")) == NULL)
   {
      fprintf(STDERR, "Could not find file %s\n", outname);
      FCLOSE(fpin);
      return (1);
   }
   for (;;)
   {
      png_size_t num_in, num_out;
         num_in = fread(inbuf, 1, 1, fpin);
         num_out = fread(outbuf, 1, 1, fpout);
      if (num_in != num_out)
      {
         fprintf(STDERR, "\nFiles %s and %s are of a different size\n",
                 inname, outname);
         if (wrote_question == 0)
         {
            fprintf(STDERR,
         "   Was %s written with the same maximum IDAT chunk size (%d bytes),",
              inname, PNG_ZBUF_SIZE);
            fprintf(STDERR,
              "\n   filtering heuristic (libpng default), compression");
            fprintf(STDERR,
              " level (zlib default),\n   and zlib version (%s)?\n\n",
              ZLIB_VERSION);
            wrote_question = 1;
         }
         FCLOSE(fpin);
         FCLOSE(fpout);
         return (0);
      }
      if (!num_in)
         break;
      if (png_memcmp(inbuf, outbuf, num_in))
      {
         fprintf(STDERR, "\nFiles %s and %s are different\n", inname, outname);
         if (wrote_question == 0)
         {
            fprintf(STDERR,
         "   Was %s written with the same maximum IDAT chunk size (%d bytes),",
                 inname, PNG_ZBUF_SIZE);
            fprintf(STDERR,
              "\n   filtering heuristic (libpng default), compression");
            fprintf(STDERR,
              " level (zlib default),\n   and zlib version (%s)?\n\n",
              ZLIB_VERSION);
            wrote_question = 1;
         }
         FCLOSE(fpin);
         FCLOSE(fpout);
         return (0);
      }
   }
   FCLOSE(fpin);
   FCLOSE(fpout);
   return (0);
}

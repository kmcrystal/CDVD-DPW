static void
make_error(png_store* volatile psIn, png_byte PNG_CONST colour_type,
    png_byte bit_depth, int interlace_type, int test, png_const_charp name)
{
   png_store * volatile ps = psIn;
   context(ps, fault);
   check_interlace_type(interlace_type);
   Try
   {
      png_structp pp;
      png_infop pi;
      pp = set_store_for_write(ps, &pi, name);
      if (pp == NULL)
         Throw ps;
      png_set_IHDR(pp, pi, transform_width(pp, colour_type, bit_depth),
         transform_height(pp, colour_type, bit_depth), bit_depth, colour_type,
         interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
      if (colour_type == 3) 
         init_standard_palette(ps, pp, pi, 1U << bit_depth, 0);
      


#     define exception__prev exception_prev_1
#     define exception__env exception_env_1
      Try
      {
         
         ps->expect_error = !error_test[test].warning;
         ps->expect_warning = error_test[test].warning;
         ps->saw_warning = 0;
         error_test[test].fn(pp, pi);
         
         png_write_info(pp, pi);
         
         if (ps->expect_warning && ps->saw_warning)
            Throw ps;
         


         store_log(ps, pp, error_test[test].msg, 1 );
      }
      Catch (fault)
         ps = fault; 
#undef exception__prev
#undef exception__env
      
      ps->expect_error = 0;
      ps->expect_warning = 0;
      


      if (png_get_rowbytes(pp, pi) !=
          transform_rowsize(pp, colour_type, bit_depth))
         png_error(pp, "row size incorrect");
      else
      {
         png_uint_32 h = transform_height(pp, colour_type, bit_depth);
         int npasses = png_set_interlace_handling(pp);
         int pass;
         if (npasses != npasses_from_interlace_type(pp, interlace_type))
            png_error(pp, "write: png_set_interlace_handling failed");
         for (pass=0; pass<npasses; ++pass)
         {
            png_uint_32 y;
            for (y=0; y<h; ++y)
            {
               png_byte buffer[TRANSFORM_ROWMAX];
               transform_row(pp, buffer, colour_type, bit_depth, y);
               png_write_row(pp, buffer);
            }
         }
      }
      png_write_end(pp, pi);
      
      store_write_reset(ps);
   }
   Catch(fault)
   {
      store_write_reset(fault);
   }
}

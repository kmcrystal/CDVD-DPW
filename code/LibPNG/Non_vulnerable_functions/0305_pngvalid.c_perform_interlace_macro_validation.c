static void
perform_interlace_macro_validation(void)
{
   






   int pass;
   for (pass=0; pass<7; ++pass)
   {
      png_uint_32 m, f, v;
      m = PNG_PASS_START_ROW(pass);
      f = png_pass_start_row(pass);
      if (m != f)
      {
         fprintf(stderr, "PNG_PASS_START_ROW(%d) = %u != %x\n", pass, m, f);
         exit(99);
      }
      m = PNG_PASS_START_COL(pass);
      f = png_pass_start_col(pass);
      if (m != f)
      {
         fprintf(stderr, "PNG_PASS_START_COL(%d) = %u != %x\n", pass, m, f);
         exit(99);
      }
      m = PNG_PASS_ROW_SHIFT(pass);
      f = png_pass_row_shift(pass);
      if (m != f)
      {
         fprintf(stderr, "PNG_PASS_ROW_SHIFT(%d) = %u != %x\n", pass, m, f);
         exit(99);
      }
      m = PNG_PASS_COL_SHIFT(pass);
      f = png_pass_col_shift(pass);
      if (m != f)
      {
         fprintf(stderr, "PNG_PASS_COL_SHIFT(%d) = %u != %x\n", pass, m, f);
         exit(99);
      }
      








      for (v=0;;)
      {
         
         m = PNG_ROW_FROM_PASS_ROW(v, pass);
         f = png_row_from_pass_row(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_ROW_FROM_PASS_ROW(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         m = PNG_COL_FROM_PASS_COL(v, pass);
         f = png_col_from_pass_col(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_COL_FROM_PASS_COL(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         m = PNG_ROW_IN_INTERLACE_PASS(v, pass);
         f = png_row_in_interlace_pass(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_ROW_IN_INTERLACE_PASS(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         m = PNG_COL_IN_INTERLACE_PASS(v, pass);
         f = png_col_in_interlace_pass(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_COL_IN_INTERLACE_PASS(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         
         ++v;
         m = PNG_PASS_ROWS(v, pass);
         f = png_pass_rows(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_PASS_ROWS(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         m = PNG_PASS_COLS(v, pass);
         f = png_pass_cols(v, pass);
         if (m != f)
         {
            fprintf(stderr, "PNG_PASS_COLS(%u, %d) = %u != %x\n",
               v, pass, m, f);
            exit(99);
         }
         


         if (v > 1024)
         {
            if (v == PNG_UINT_31_MAX)
               break;
            v = (v << 1) ^ v;
            if (v >= PNG_UINT_31_MAX)
               v = PNG_UINT_31_MAX-1;
         }
      }
   }
}

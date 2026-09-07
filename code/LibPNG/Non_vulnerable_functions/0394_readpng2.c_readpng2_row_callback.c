}
static void readpng2_row_callback(png_structp png_ptr, png_bytep new_row,
                                  png_uint_32 row_num, int pass)
{
    mainprog_info  *mainprog_ptr;
    

    if (!new_row)
        return;
    

    mainprog_ptr = png_get_progressive_ptr(png_ptr);
    
    mainprog_ptr->pass = pass;
    


    png_progressive_combine_row(png_ptr, mainprog_ptr->row_pointers[row_num],
      new_row);
    

    (*mainprog_ptr->mainprog_display_row)(row_num);
    
    return;
}

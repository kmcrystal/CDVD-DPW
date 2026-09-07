}
static void readpng2_end_callback(png_structp png_ptr, png_infop info_ptr)
{
    mainprog_info  *mainprog_ptr;
    
    mainprog_ptr = png_get_progressive_ptr(png_ptr);
    





    (*mainprog_ptr->mainprog_finish_display)();
    
    return;
}

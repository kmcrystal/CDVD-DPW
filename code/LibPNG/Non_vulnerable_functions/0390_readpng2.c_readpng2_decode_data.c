
int readpng2_decode_data(mainprog_info *mainprog_ptr, uch *rawbuf, ulg length)
{
    png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
    png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;
    

    if (setjmp(mainprog_ptr->jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        mainprog_ptr->png_ptr = NULL;
        mainprog_ptr->info_ptr = NULL;
        return 2;
    }
    
    png_process_data(png_ptr, info_ptr, rawbuf, length);
    return 0;
}

}
void writepng_cleanup(mainprog_info *mainprog_ptr)
{
    png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
    png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;
    if (png_ptr && info_ptr)
        png_destroy_write_struct(&png_ptr, &info_ptr);
}

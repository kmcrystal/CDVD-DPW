
int readpng_init(FILE *infile, ulg *pWidth, ulg *pHeight)
{
    uch sig[8];
    

    fread(sig, 1, 8, infile);
    if (png_sig_cmp(sig, 0, 8))
        return 1;   
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        return 4;   
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 4;   
    }
    


    

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }
    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);  
    png_read_info(png_ptr, info_ptr);  
    


    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
      NULL, NULL, NULL);
    *pWidth = width;
    *pHeight = height;
    
    return 0;
}

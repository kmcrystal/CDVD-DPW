}
static void readpng2_info_callback(png_structp png_ptr, png_infop info_ptr)
{
    mainprog_info  *mainprog_ptr;
    int  color_type, bit_depth;
    png_uint_32 width, height;
#ifdef PNG_FLOATING_POINT_SUPPORTED
    double  gamma;
#else
    png_fixed_point gamma;
#endif
    




    




    mainprog_ptr = png_get_progressive_ptr(png_ptr);
    if (mainprog_ptr == NULL) {         
        fprintf(stderr,
          "readpng2 error:  main struct not recoverable in info_callback.\n");
        fflush(stderr);
        return;
        






    }
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
       NULL, NULL, NULL);
    mainprog_ptr->width = (ulg)width;
    mainprog_ptr->height = (ulg)height;
    

    if (mainprog_ptr->need_bgcolor &&
        png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD))
    {
        png_color_16p pBackground;
        


        png_get_bKGD(png_ptr, info_ptr, &pBackground);
        

        if (bit_depth == 16) {
            mainprog_ptr->bg_red   = pBackground->red   >> 8;
            mainprog_ptr->bg_green = pBackground->green >> 8;
            mainprog_ptr->bg_blue  = pBackground->blue  >> 8;
        } else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            if (bit_depth == 1)
                mainprog_ptr->bg_red = mainprog_ptr->bg_green =
                  mainprog_ptr->bg_blue = pBackground->gray? 255 : 0;
            else if (bit_depth == 2)
                mainprog_ptr->bg_red = mainprog_ptr->bg_green =
                  mainprog_ptr->bg_blue = (255/3) * pBackground->gray;
            else 
                mainprog_ptr->bg_red = mainprog_ptr->bg_green =
                  mainprog_ptr->bg_blue = (255/15) * pBackground->gray;
        } else {
            mainprog_ptr->bg_red   = (uch)pBackground->red;
            mainprog_ptr->bg_green = (uch)pBackground->green;
            mainprog_ptr->bg_blue  = (uch)pBackground->blue;
        }
    }
    



    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand(png_ptr);
#ifdef PNG_READ_16_TO_8_SUPPORTED
    if (bit_depth == 16)
#  ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
        png_set_scale_16(png_ptr);
#  else
        png_set_strip_16(png_ptr);
#  endif
#endif
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    













#ifdef PNG_FLOATING_POINT_SUPPORTED
    if (png_get_gAMA(png_ptr, info_ptr, &gamma))
        png_set_gamma(png_ptr, mainprog_ptr->display_exponent, gamma);
    else
        png_set_gamma(png_ptr, mainprog_ptr->display_exponent, 0.45455);
#else
    if (png_get_gAMA_fixed(png_ptr, info_ptr, &gamma))
        png_set_gamma_fixed(png_ptr,
            (png_fixed_point)(100000*mainprog_ptr->display_exponent+.5), gamma);
    else
        png_set_gamma_fixed(png_ptr,
            (png_fixed_point)(100000*mainprog_ptr->display_exponent+.5), 45455);
#endif
    
    mainprog_ptr->passes = png_set_interlace_handling(png_ptr);
    

    png_read_update_info(png_ptr, info_ptr);
    mainprog_ptr->rowbytes = (int)png_get_rowbytes(png_ptr, info_ptr);
    mainprog_ptr->channels = png_get_channels(png_ptr, info_ptr);
    



    (*mainprog_ptr->mainprog_init)();
    
    return;
}

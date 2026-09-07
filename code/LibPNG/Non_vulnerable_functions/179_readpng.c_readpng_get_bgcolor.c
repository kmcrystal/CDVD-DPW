 * scales values to 8-bit if necessary */
int readpng_get_bgcolor(uch *red, uch *green, uch *blue)
{
    png_color_16p pBackground;
    

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }
    if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD))
        return 1;
    


    png_get_bKGD(png_ptr, info_ptr, &pBackground);
    

    if (bit_depth == 16) {
        *red   = pBackground->red   >> 8;
        *green = pBackground->green >> 8;
        *blue  = pBackground->blue  >> 8;
    } else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        if (bit_depth == 1)
            *red = *green = *blue = pBackground->gray? 255 : 0;
        else if (bit_depth == 2)
            *red = *green = *blue = (255/3) * pBackground->gray;
        else 
            *red = *green = *blue = (255/15) * pBackground->gray;
    } else {
        *red   = (uch)pBackground->red;
        *green = (uch)pBackground->green;
        *blue  = (uch)pBackground->blue;
    }
    return 0;
}


uch *readpng_get_image(double display_exponent, int *pChannels, ulg *pRowbytes)
{
    double  gamma;
    png_uint_32  i, rowbytes;
    png_bytepp  row_pointers = NULL;
    

    if (setjmp(png_jmpbuf(png_ptr))) {
        free(image_data);
        image_data = NULL;
        free(row_pointers);
        row_pointers = NULL;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
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
    


    if (png_get_gAMA(png_ptr, info_ptr, &gamma))
        png_set_gamma(png_ptr, display_exponent, gamma);
    

    png_read_update_info(png_ptr, info_ptr);
    *pRowbytes = rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    *pChannels = (int)png_get_channels(png_ptr, info_ptr);
    if ((image_data = (uch *)malloc(rowbytes*height)) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    if ((row_pointers = (png_bytepp)malloc(height*sizeof(png_bytep))) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        image_data = NULL;
        return NULL;
    }
    Trace((stderr, "readpng_get_image:  channels = %d, rowbytes = %ld, height = %ld\n",
        *pChannels, rowbytes, height));
    
    for (i = 0;  i < height;  ++i)
        row_pointers[i] = image_data + i*rowbytes;
    
    png_read_image(png_ptr, row_pointers);
    

    free(row_pointers);
    row_pointers = NULL;
    png_read_end(png_ptr, NULL);
    return image_data;
}

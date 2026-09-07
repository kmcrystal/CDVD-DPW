
BOOL PngLoadImage (PTSTR pstrFileName, png_byte **ppbImageData,
                   int *piWidth, int *piHeight, int *piChannels, png_color *pBkgColor)
{
    static FILE        *pfFile;
    png_byte            pbSig[8];
    int                 iBitDepth;
    int                 iColorType;
    double              dGamma;
    png_color_16       *pBackground;
    png_uint_32         ulChannels;
    png_uint_32         ulRowBytes;
    png_byte           *pbImageData = *ppbImageData;
    static png_byte   **ppbRowPointers = NULL;
    int                 i;
    
    if (!pstrFileName)
    {
        *ppbImageData = pbImageData = NULL;
        return FALSE;
    }
    if (!(pfFile = fopen(pstrFileName, "rb")))
    {
        *ppbImageData = pbImageData = NULL;
        return FALSE;
    }
    
    fread(pbSig, 1, 8, pfFile);
    if (png_sig_cmp(pbSig, 0, 8))
    {
        *ppbImageData = pbImageData = NULL;
        return FALSE;
    }
    
    png_ptr = png_create_read_struct(png_get_libpng_ver(NULL), NULL,
      (png_error_ptr)png_cexcept_error, (png_error_ptr)NULL);
    if (!png_ptr)
    {
        *ppbImageData = pbImageData = NULL;
        return FALSE;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        *ppbImageData = pbImageData = NULL;
        return FALSE;
    }
    Try
    {
        
#ifdef PNG_STDIO_SUPPORTED
        png_init_io(png_ptr, pfFile);
#else
        png_set_read_fn(png_ptr, (png_voidp)pfFile, png_read_data);
#endif
        png_set_sig_bytes(png_ptr, 8);
        
        png_read_info(png_ptr, info_ptr);
        
        png_get_IHDR(png_ptr, info_ptr, piWidth, piHeight, &iBitDepth,
            &iColorType, NULL, NULL, NULL);
        
        
#ifdef PNG_READ_16_TO_8_SUPPORTED
    if (iBitDepth == 16)
#  ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
        png_set_scale_16(png_ptr);
#  else
        png_set_strip_16(png_ptr);
#  endif
#endif
        if (iColorType == PNG_COLOR_TYPE_PALETTE)
            png_set_expand(png_ptr);
        if (iBitDepth < 8)
            png_set_expand(png_ptr);
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            png_set_expand(png_ptr);
        if (iColorType == PNG_COLOR_TYPE_GRAY ||
            iColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png_ptr);
        
        if (png_get_bKGD(png_ptr, info_ptr, &pBackground))
        {
            png_set_background(png_ptr, pBackground, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
            pBkgColor->red   = (byte) pBackground->red;
            pBkgColor->green = (byte) pBackground->green;
            pBkgColor->blue  = (byte) pBackground->blue;
        }
        else
        {
            pBkgColor = NULL;
        }
        
        if (png_get_gAMA(png_ptr, info_ptr, &dGamma))
            png_set_gamma(png_ptr, (double) 2.2, dGamma);
        
        png_read_update_info(png_ptr, info_ptr);
        
        png_get_IHDR(png_ptr, info_ptr, piWidth, piHeight, &iBitDepth,
            &iColorType, NULL, NULL, NULL);
        
        ulRowBytes = png_get_rowbytes(png_ptr, info_ptr);
        ulChannels = png_get_channels(png_ptr, info_ptr);
        *piChannels = ulChannels;
        
        if (pbImageData)
        {
            free (pbImageData);
            pbImageData = NULL;
        }
        if ((pbImageData = (png_byte *) malloc(ulRowBytes * (*piHeight)
                            * sizeof(png_byte))) == NULL)
        {
            png_error(png_ptr, "Visual PNG: out of memory");
        }
        *ppbImageData = pbImageData;
        
        if ((ppbRowPointers = (png_bytepp) malloc((*piHeight)
                            * sizeof(png_bytep))) == NULL)
        {
            png_error(png_ptr, "Visual PNG: out of memory");
        }
        
        for (i = 0; i < (*piHeight); i++)
            ppbRowPointers[i] = pbImageData + i * ulRowBytes;
        
        png_read_image(png_ptr, ppbRowPointers);
        
        png_read_end(png_ptr, NULL);
        
        free (ppbRowPointers);
        ppbRowPointers = NULL;
        
    }
    Catch (msg)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        *ppbImageData = pbImageData = NULL;
        if(ppbRowPointers)
            free (ppbRowPointers);
        fclose(pfFile);
        return FALSE;
    }
    fclose (pfFile);
    return TRUE;
}

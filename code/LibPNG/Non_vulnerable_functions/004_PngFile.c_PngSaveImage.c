}
BOOL PngSaveImage (PTSTR pstrFileName, png_byte *pDiData,
                   int iWidth, int iHeight, png_color bkgColor)
{
    const int           ciBitDepth = 8;
    const int           ciChannels = 3;
    static FILE        *pfFile;
    png_uint_32         ulRowBytes;
    static png_byte   **ppbRowPointers = NULL;
    int                 i;
    
    if (!pstrFileName)
        return FALSE;
    if (!(pfFile = fopen(pstrFileName, "wb")))
        return FALSE;
    
    png_ptr = png_create_write_struct(png_get_libpng_ver(NULL), NULL,
      (png_error_ptr)png_cexcept_error, (png_error_ptr)NULL);
    if (!png_ptr)
    {
        fclose(pfFile);
        return FALSE;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(pfFile);
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        return FALSE;
    }
    Try
    {
        
#ifdef PNG_STDIO_SUPPORTED
        png_init_io(png_ptr, pfFile);
#else
        png_set_write_fn(png_ptr, (png_voidp)pfFile, png_write_data, png_flush);
#endif
        
        png_set_IHDR(png_ptr, info_ptr, iWidth, iHeight, ciBitDepth,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE);
        
        png_write_info(png_ptr, info_ptr);
        
        png_set_bgr(png_ptr);
        
        ulRowBytes = iWidth * ciChannels;
        
        if ((ppbRowPointers = (png_bytepp) malloc(iHeight * sizeof(png_bytep))) == NULL)
            Throw "Visualpng: Out of memory";
        
        for (i = 0; i < iHeight; i++)
            ppbRowPointers[i] = pDiData + i * (((ulRowBytes + 3) >> 2) << 2);
        
        png_write_image (png_ptr, ppbRowPointers);
        
        png_write_end(png_ptr, info_ptr);
        
        free (ppbRowPointers);
        ppbRowPointers = NULL;
        
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        
    }
    Catch (msg)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        if(ppbRowPointers)
            free (ppbRowPointers);
        fclose(pfFile);
        return FALSE;
    }
    fclose (pfFile);
    return TRUE;
}

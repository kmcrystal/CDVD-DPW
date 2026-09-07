static void 
cleanup_and_exit(void)
{
        TIFFRGBAImageEnd(&img);
        if (filelist != NULL)
                _TIFFfree(filelist);
        if (raster != NULL)
                _TIFFfree(raster);
        if (tif != NULL)
                TIFFClose(tif);
        exit(0);
}

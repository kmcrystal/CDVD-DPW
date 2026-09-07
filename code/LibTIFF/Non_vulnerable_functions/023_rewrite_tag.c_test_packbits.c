const uint32	rows_per_strip = 1;
int test_packbits()
{
    TIFF	    *tif;
    int             i;
    unsigned char   buf[10] = {0,0,0,0,0,0,0,0,0,0};
    int  length = 20;
    const char *filename = "test_packbits.tif";
    
    tif = TIFFOpen(filename, "w");
    if (!tif) {
        fprintf (stderr, "Can't create test TIFF file %s.\n", filename);
        return 1;
    }
    if (!TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS)) {
        fprintf (stderr, "Can't set Compression tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width)) {
        fprintf (stderr, "Can't set ImageWidth tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length)) {
        fprintf (stderr, "Can't set ImageLength tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8)) {
        fprintf (stderr, "Can't set BitsPerSample tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1)) {
        fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip)) {
        fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
        goto failure;
    }
    if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) {
        fprintf (stderr, "Can't set PlanarConfiguration tag.\n");
        goto failure;
    }
    for (i = 0; i < length; i++ )
    {
        if( !TIFFWriteEncodedStrip( tif, i, buf, 10 ) )
        {
            fprintf (stderr, "Can't write image data.\n");
            goto failure;
        }
    }
    TIFFClose(tif);
    
    tif = TIFFOpen(filename, "r+");
    if (!tif) {
        fprintf (stderr, "Can't create test TIFF file %s.\n", filename);
        return 1;
    }
    buf[3] = 17;
    buf[6] = 12;
    if( !TIFFWriteEncodedStrip( tif, 6, buf, 10 ) )
    {
        fprintf (stderr, "Can't write image data.\n");
        goto failure;
    }
    TIFFClose(tif);
    unlink(filename);
    return 0;
  failure:
    
    TIFFClose(tif);
    
    return 1;
}

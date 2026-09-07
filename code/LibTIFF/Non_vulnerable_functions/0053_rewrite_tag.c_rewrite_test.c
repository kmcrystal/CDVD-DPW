
int rewrite_test( const char *filename, int length, int bigtiff, 
                  uint64 base_value )
{
    TIFF		*tif;
    int			i;
    unsigned char	buf[10] = {5,6,7,8,9,10,11,12,13,14};
    uint64		*rowoffset, *rowbytes;
    uint64		*upd_rowoffset;
    uint64		*upd_bytecount;
    
    if( bigtiff )
        tif = TIFFOpen(filename, "w8");
    else
        tif = TIFFOpen(filename, "w4");
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
        if( TIFFWriteScanline( tif, buf, i, 0 ) == -1 )
        {
            fprintf (stderr, "Can't write image data.\n");
            goto failure;
        }
    }
    TIFFClose(tif);
    
    tif = TIFFOpen(filename, "r+");
    if (!tif) {
        fprintf (stderr, "Can't open test TIFF file %s.\n", filename);
        return 1;
    }
    if( !TIFFGetField( tif, TIFFTAG_STRIPOFFSETS, &rowoffset ) )
    {
        fprintf (stderr, "Can't fetch STRIPOFFSETS.\n");
        goto failure;
    }
    if( !TIFFGetField( tif, TIFFTAG_STRIPBYTECOUNTS, &rowbytes ) )
    {
        fprintf (stderr, "Can't fetch STRIPBYTECOUNTS.\n");
        goto failure;
    }
    upd_rowoffset = (uint64 *) _TIFFmalloc(sizeof(uint64) * length);
    for( i = 0; i < length; i++ )
        upd_rowoffset[i] = base_value + i*10;
    if( !_TIFFRewriteField( tif, TIFFTAG_STRIPOFFSETS, TIFF_LONG8, 
                            length, upd_rowoffset ) )
    {
        fprintf (stderr, "Can't rewrite STRIPOFFSETS.\n");
        goto failure;
    }
    _TIFFfree( upd_rowoffset );
    upd_bytecount = (uint64 *) _TIFFmalloc(sizeof(uint64) * length);
    for( i = 0; i < length; i++ )
        upd_bytecount[i] = 100 + i*10;
    if( !_TIFFRewriteField( tif, TIFFTAG_STRIPBYTECOUNTS, TIFF_LONG8, 
                            length, upd_bytecount ) )
    {
        fprintf (stderr, "Can't rewrite STRIPBYTECOUNTS.\n");
        goto failure;
    }
    _TIFFfree( upd_bytecount );
    TIFFClose(tif);
    
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        fprintf (stderr, "Can't open test TIFF file %s.\n", filename);
        return 1;
    }
    if( !TIFFGetField( tif, TIFFTAG_STRIPOFFSETS, &rowoffset ) )
    {
        fprintf (stderr, "Can't fetch STRIPOFFSETS.\n");
        goto failure;
    }
    for( i = 0; i < length; i++ )
    {
        uint64 expect = base_value + i*10;
        if( rowoffset[i] != expect )
        {
            fprintf( stderr, 
                     "%s:STRIPOFFSETS[%d]: Got %X:%08X instead of %X:%08X.\n",
                     filename, i, 
                     (int) (rowoffset[i] >> 32), 
                     (int) (rowoffset[i]&0xFFFFFFFF), 
                     (int) (expect >> 32), 
                     (int) (expect & 0xFFFFFFFF) );
            goto failure;
        }
    }
    if( !TIFFGetField( tif, TIFFTAG_STRIPBYTECOUNTS, &rowbytes ) )
    {
        fprintf (stderr, "Can't fetch STRIPBYTECOUNTS.\n");
        goto failure;
    }
    for( i = 0; i < length; i++ )
    {
        uint64 expect = 100 + i*10;
        if( rowbytes[i] != expect )
        {
            fprintf( stderr, 
                     "%s:STRIPBYTECOUNTS[%d]: Got %X:%08X instead of %X:%08X.\n",
                     filename, i, 
                     (int) (rowbytes[i] >> 32), 
                     (int) (rowbytes[i] & 0xFFFFFFFF), 
                     (int) (expect >> 32), 
                     (int) (expect & 0xFFFFFFFF) );
            goto failure;
        }
    }
    TIFFClose( tif );
    
    unlink(filename);
    return 0;
  failure:
    
    TIFFClose(tif);
    
    return 1;
}

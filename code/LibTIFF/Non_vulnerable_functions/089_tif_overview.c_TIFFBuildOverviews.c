
void TIFFBuildOverviews( TIFF *hTIFF, int nOverviews, int * panOvList,
                         int bUseSubIFDs, const char *pszResampleMethod,
                         int (*pfnProgress)( double, void * ),
                         void * pProgressData )
{
    TIFFOvrCache	**papoRawBIs;
    uint32		nXSize, nYSize, nBlockXSize, nBlockYSize;
    uint16		nBitsPerPixel, nPhotometric, nCompressFlag, nSamples,
        nPlanarConfig, nSampleFormat;
    int         bSubsampled;
    uint16      nHorSubsampling, nVerSubsampling;
    int			bTiled, nSXOff, nSYOff, i;
    unsigned char	*pabySrcTile;
    uint16		*panRedMap, *panGreenMap, *panBlueMap;
    TIFFErrorHandler    pfnWarning;
    (void) pfnProgress;
    (void) pProgressData;



    TIFFGetField( hTIFF, TIFFTAG_IMAGEWIDTH, &nXSize );
    TIFFGetField( hTIFF, TIFFTAG_IMAGELENGTH, &nYSize );
    TIFFGetField( hTIFF, TIFFTAG_BITSPERSAMPLE, &nBitsPerPixel );
    
    TIFFGetField( hTIFF, TIFFTAG_SAMPLESPERPIXEL, &nSamples );
    TIFFGetFieldDefaulted( hTIFF, TIFFTAG_PLANARCONFIG, &nPlanarConfig );
    TIFFGetFieldDefaulted( hTIFF, TIFFTAG_PHOTOMETRIC, &nPhotometric );
    TIFFGetFieldDefaulted( hTIFF, TIFFTAG_COMPRESSION, &nCompressFlag );
    TIFFGetFieldDefaulted( hTIFF, TIFFTAG_SAMPLEFORMAT, &nSampleFormat );
    if( nPhotometric == PHOTOMETRIC_YCBCR || nPhotometric == PHOTOMETRIC_ITULAB )
    {
        if( nBitsPerPixel != 8 || nSamples != 3 || nPlanarConfig != PLANARCONFIG_CONTIG ||
            nSampleFormat != SAMPLEFORMAT_UINT)
        {
            
            TIFFErrorExt( TIFFClientdata(hTIFF), "TIFFBuildOverviews",
                          "File `%s' has an unsupported subsampling configuration.\n",
                          TIFFFileName(hTIFF) );
            



            return;
        }
        bSubsampled = 1;
        TIFFGetField( hTIFF, TIFFTAG_YCBCRSUBSAMPLING, &nHorSubsampling, &nVerSubsampling );
        
    }
    else
    {
        if( nBitsPerPixel < 8 )
        {
            
            TIFFErrorExt( TIFFClientdata(hTIFF), "TIFFBuildOverviews",
                          "File `%s' has samples of %d bits per sample.  Sample\n"
                          "sizes of less than 8 bits per sample are not supported.\n",
                          TIFFFileName(hTIFF), nBitsPerPixel );
            return;
        }
        bSubsampled = 0;
        nHorSubsampling = 1;
        nVerSubsampling = 1;
    }




    pfnWarning = TIFFSetWarningHandler( NULL );



    if( TIFFGetField( hTIFF, TIFFTAG_ROWSPERSTRIP, &(nBlockYSize) ) )
    {
        nBlockXSize = nXSize;
        bTiled = FALSE;
    }
    else
    {
        TIFFGetField( hTIFF, TIFFTAG_TILEWIDTH, &nBlockXSize );
        TIFFGetField( hTIFF, TIFFTAG_TILELENGTH, &nBlockYSize );
        bTiled = TRUE;
    }



    if( TIFFGetField( hTIFF, TIFFTAG_COLORMAP,
                      &panRedMap, &panGreenMap, &panBlueMap ) )
    {
        uint16		*panRed2, *panGreen2, *panBlue2;
        int             nColorCount = 1 << nBitsPerPixel;
        panRed2 = (uint16 *) _TIFFmalloc(2*nColorCount);
        panGreen2 = (uint16 *) _TIFFmalloc(2*nColorCount);
        panBlue2 = (uint16 *) _TIFFmalloc(2*nColorCount);
        memcpy( panRed2, panRedMap, 2 * nColorCount );
        memcpy( panGreen2, panGreenMap, 2 * nColorCount );
        memcpy( panBlue2, panBlueMap, 2 * nColorCount );
        panRedMap = panRed2;
        panGreenMap = panGreen2;
        panBlueMap = panBlue2;
    }
    else
    {
        panRedMap = panGreenMap = panBlueMap = NULL;
    }



    papoRawBIs = (TIFFOvrCache **) _TIFFmalloc(nOverviews*sizeof(void*));
    for( i = 0; i < nOverviews; i++ )
    {
        uint32  nOXSize, nOYSize, nOBlockXSize, nOBlockYSize;
        toff_t  nDirOffset;
        nOXSize = (nXSize + panOvList[i] - 1) / panOvList[i];
        nOYSize = (nYSize + panOvList[i] - 1) / panOvList[i];
        nOBlockXSize = MIN(nBlockXSize,nOXSize);
        nOBlockYSize = MIN(nBlockYSize,nOYSize);
        if( bTiled )
        {
            if( (nOBlockXSize % 16) != 0 )
                nOBlockXSize = nOBlockXSize + 16 - (nOBlockXSize % 16);
            if( (nOBlockYSize % 16) != 0 )
                nOBlockYSize = nOBlockYSize + 16 - (nOBlockYSize % 16);
        }
        nDirOffset = TIFF_WriteOverview( hTIFF, nOXSize, nOYSize,
                                         nBitsPerPixel, nPlanarConfig,
                                         nSamples, nOBlockXSize, nOBlockYSize,
                                         bTiled, nCompressFlag, nPhotometric,
                                         nSampleFormat,
                                         panRedMap, panGreenMap, panBlueMap,
                                         bUseSubIFDs,
                                         nHorSubsampling, nVerSubsampling );
        papoRawBIs[i] = TIFFCreateOvrCache( hTIFF, nDirOffset );
    }
    if( panRedMap != NULL )
    {
        _TIFFfree( panRedMap );
        _TIFFfree( panGreenMap );
        _TIFFfree( panBlueMap );
    }



    if( bTiled )
        pabySrcTile = (unsigned char *) _TIFFmalloc(TIFFTileSize(hTIFF));
    else
        pabySrcTile = (unsigned char *) _TIFFmalloc(TIFFStripSize(hTIFF));




    for( nSYOff = 0; nSYOff < (int) nYSize; nSYOff += nBlockYSize )
    {
        for( nSXOff = 0; nSXOff < (int) nXSize; nSXOff += nBlockXSize )
        {
            


            TIFF_ProcessFullResBlock( hTIFF, nPlanarConfig,
                                      bSubsampled,nHorSubsampling,nVerSubsampling,
                                      nOverviews, panOvList,
                                      nBitsPerPixel, nSamples, papoRawBIs,
                                      nSXOff, nSYOff, pabySrcTile,
                                      nBlockXSize, nBlockYSize,
                                      nSampleFormat, pszResampleMethod );
        }
    }
    _TIFFfree( pabySrcTile );



    for( i = 0; i < nOverviews; i++ )
    {
        TIFFDestroyOvrCache( papoRawBIs[i] );
    }
    if( papoRawBIs != NULL )
        _TIFFfree( papoRawBIs );
    TIFFSetWarningHandler( pfnWarning );
}

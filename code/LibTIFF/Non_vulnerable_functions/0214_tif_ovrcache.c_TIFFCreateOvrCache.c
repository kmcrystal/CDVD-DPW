
TIFFOvrCache *TIFFCreateOvrCache( TIFF *hTIFF, toff_t nDirOffset )
{
    TIFFOvrCache	*psCache;
    toff_t		nBaseDirOffset;
    psCache = (TIFFOvrCache *) _TIFFmalloc(sizeof(TIFFOvrCache));
    psCache->nDirOffset = nDirOffset;
    psCache->hTIFF = hTIFF;



    nBaseDirOffset = TIFFCurrentDirOffset( psCache->hTIFF );
    TIFFSetSubDirectory( hTIFF, nDirOffset );
    TIFFGetField( hTIFF, TIFFTAG_IMAGEWIDTH, &(psCache->nXSize) );
    TIFFGetField( hTIFF, TIFFTAG_IMAGELENGTH, &(psCache->nYSize) );
    TIFFGetField( hTIFF, TIFFTAG_BITSPERSAMPLE, &(psCache->nBitsPerPixel) );
    TIFFGetField( hTIFF, TIFFTAG_SAMPLESPERPIXEL, &(psCache->nSamples) );
    TIFFGetField( hTIFF, TIFFTAG_PLANARCONFIG, &(psCache->nPlanarConfig) );
    if( !TIFFIsTiled( hTIFF ) )
    {
        TIFFGetField( hTIFF, TIFFTAG_ROWSPERSTRIP, &(psCache->nBlockYSize) );
        psCache->nBlockXSize = psCache->nXSize;
        psCache->nBytesPerBlock = TIFFStripSize(hTIFF);
        psCache->bTiled = FALSE;
    }
    else
    {
        TIFFGetField( hTIFF, TIFFTAG_TILEWIDTH, &(psCache->nBlockXSize) );
        TIFFGetField( hTIFF, TIFFTAG_TILELENGTH, &(psCache->nBlockYSize) );
        psCache->nBytesPerBlock = TIFFTileSize(hTIFF);
        psCache->bTiled = TRUE;
    }



    psCache->nBlocksPerRow = (psCache->nXSize + psCache->nBlockXSize - 1)
        		/ psCache->nBlockXSize;
    psCache->nBlocksPerColumn = (psCache->nYSize + psCache->nBlockYSize - 1)
        		/ psCache->nBlockYSize;
    if (psCache->nPlanarConfig == PLANARCONFIG_SEPARATE)
        psCache->nBytesPerRow = psCache->nBytesPerBlock
            * psCache->nBlocksPerRow * psCache->nSamples;
    else
        psCache->nBytesPerRow =
            psCache->nBytesPerBlock * psCache->nBlocksPerRow;



    psCache->pabyRow1Blocks =
        (unsigned char *) _TIFFmalloc(psCache->nBytesPerRow);
    psCache->pabyRow2Blocks =
        (unsigned char *) _TIFFmalloc(psCache->nBytesPerRow);
    if( psCache->pabyRow1Blocks == NULL
        || psCache->pabyRow2Blocks == NULL )
    {
		TIFFErrorExt( hTIFF->tif_clientdata, hTIFF->tif_name,
					  "Can't allocate memory for overview cache." );
        
        return NULL;
    }
    _TIFFmemset( psCache->pabyRow1Blocks, 0, psCache->nBytesPerRow );
    _TIFFmemset( psCache->pabyRow2Blocks, 0, psCache->nBytesPerRow );
    psCache->nBlockOffset = 0;
    TIFFSetSubDirectory( psCache->hTIFF, nBaseDirOffset );
    return psCache;
}

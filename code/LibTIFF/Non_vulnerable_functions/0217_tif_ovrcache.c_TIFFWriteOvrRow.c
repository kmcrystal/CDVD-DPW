
static void TIFFWriteOvrRow( TIFFOvrCache * psCache )
{
    int		nRet, iTileX, iTileY = psCache->nBlockOffset;
    unsigned char *pabyData;
    toff_t	nBaseDirOffset;
    uint32      RowsInStrip;





    if( TIFFIsByteSwapped(psCache->hTIFF) )
    {
        if( psCache->nBitsPerPixel == 16 )
            TIFFSwabArrayOfShort( (uint16 *) psCache->pabyRow1Blocks,
                      (psCache->nBytesPerBlock * psCache->nSamples) / 2 );
        else if( psCache->nBitsPerPixel == 32 )
            TIFFSwabArrayOfLong( (uint32 *) psCache->pabyRow1Blocks,
                         (psCache->nBytesPerBlock * psCache->nSamples) / 4 );
        else if( psCache->nBitsPerPixel == 64 )
            TIFFSwabArrayOfDouble( (double *) psCache->pabyRow1Blocks,
                         (psCache->nBytesPerBlock * psCache->nSamples) / 8 );
    }




    nBaseDirOffset = TIFFCurrentDirOffset( psCache->hTIFF );
    nRet = TIFFSetSubDirectory( psCache->hTIFF, psCache->nDirOffset );
    assert( nRet == 1 );



	for( iTileX = 0; iTileX < psCache->nBlocksPerRow; iTileX++ )
	{
		int nTileID;
		if (psCache->nPlanarConfig == PLANARCONFIG_SEPARATE)
		{
			int iSample;
			for( iSample = 0; iSample < psCache->nSamples; iSample++ )
			{
				pabyData = TIFFGetOvrBlock( psCache, iTileX, iTileY, iSample );
				if( psCache->bTiled )
				{
					nTileID = TIFFComputeTile( psCache->hTIFF,
					    iTileX * psCache->nBlockXSize,
					    iTileY * psCache->nBlockYSize,
					    0, (tsample_t) iSample );
					TIFFWriteEncodedTile( psCache->hTIFF, nTileID,
					    pabyData,
					    TIFFTileSize(psCache->hTIFF) );
				}
				else
				{
					nTileID = TIFFComputeStrip( psCache->hTIFF,
					    iTileY * psCache->nBlockYSize,
					    (tsample_t) iSample );
					RowsInStrip=psCache->nBlockYSize;
					if ((iTileY+1)*psCache->nBlockYSize>psCache->nYSize)
						RowsInStrip=psCache->nYSize-iTileY*psCache->nBlockYSize;
					TIFFWriteEncodedStrip( psCache->hTIFF, nTileID,
					    pabyData,
					    TIFFVStripSize(psCache->hTIFF,RowsInStrip) );
				}
			}
		}
		else
		{
			pabyData = TIFFGetOvrBlock( psCache, iTileX, iTileY, 0 );
			if( psCache->bTiled )
			{
				nTileID = TIFFComputeTile( psCache->hTIFF,
				    iTileX * psCache->nBlockXSize,
				    iTileY * psCache->nBlockYSize,
				    0, 0 );
				TIFFWriteEncodedTile( psCache->hTIFF, nTileID,
				    pabyData,
				    TIFFTileSize(psCache->hTIFF) );
			}
			else
			{
				nTileID = TIFFComputeStrip( psCache->hTIFF,
				    iTileY * psCache->nBlockYSize,
				    0 );
				RowsInStrip=psCache->nBlockYSize;
				if ((iTileY+1)*psCache->nBlockYSize>psCache->nYSize)
					RowsInStrip=psCache->nYSize-iTileY*psCache->nBlockYSize;
				TIFFWriteEncodedStrip( psCache->hTIFF, nTileID,
				    pabyData,
				    TIFFVStripSize(psCache->hTIFF,RowsInStrip) );
			}
		}
	}
	



    pabyData = psCache->pabyRow1Blocks;
    psCache->pabyRow1Blocks = psCache->pabyRow2Blocks;
    psCache->pabyRow2Blocks = pabyData;
    _TIFFmemset( pabyData, 0, psCache->nBytesPerRow );
    psCache->nBlockOffset++;



    TIFFFlush( psCache->hTIFF );
    
    TIFFSetSubDirectory( psCache->hTIFF, nBaseDirOffset );
    
}

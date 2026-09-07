static
void TIFF_DownSample( unsigned char *pabySrcTile,
                      uint32 nBlockXSize, uint32 nBlockYSize,
                      int nPixelSkewBits, int nBitsPerPixel,
                      unsigned char * pabyOTile,
                      uint32 nOBlockXSize, uint32 nOBlockYSize,
                      uint32 nTXOff, uint32 nTYOff, int nOMult,
                      int nSampleFormat, const char * pszResampling )
{
    uint32	i, j;
    int         k, nPixelBytes = (nBitsPerPixel) / 8;
    int		nPixelGroupBytes = (nBitsPerPixel+nPixelSkewBits)/8;
    unsigned char *pabySrc, *pabyDst;
    double      *padfSamples;
    assert( nBitsPerPixel >= 8 );
    padfSamples = (double *) malloc(sizeof(double) * nOMult * nOMult);




    for( j = 0; j*nOMult < nBlockYSize; j++ )
    {
        if( j + nTYOff >= nOBlockYSize )
            break;
        pabyDst = pabyOTile + ((j+nTYOff)*nOBlockXSize + nTXOff)
            * nPixelBytes * nPixelGroupBytes;




        if( strncmp(pszResampling,"nearest",4) == 0
            || strncmp(pszResampling,"NEAR",4) == 0 )
        {
            pabySrc = pabySrcTile + j*nOMult*nBlockXSize * nPixelGroupBytes;
            for( i = 0; i*nOMult < nBlockXSize; i++ )
            {
                if( i + nTXOff >= nOBlockXSize )
                    break;
                



                for( k = 0; k < nPixelBytes; k++ )
                    pabyDst[k] = pabySrc[k];
                pabyDst += nPixelBytes * nPixelGroupBytes;
                pabySrc += nOMult * nPixelGroupBytes;
            }
        }




        else if( strncmp(pszResampling,"averag",6) == 0
                 || strncmp(pszResampling,"AVERAG",6) == 0 )
        {
            pabySrc = pabySrcTile + j*nOMult*nBlockXSize * nPixelGroupBytes;
            for( i = 0; i*nOMult < nBlockXSize; i++ )
            {
                double   dfTotal;
                uint32   nXSize, nYSize, iSample;
                if( i + nTXOff >= nOBlockXSize )
                    break;
                nXSize = MIN((uint32)nOMult,nBlockXSize-i);
                nYSize = MIN((uint32)nOMult,nBlockYSize-j);
                TIFF_GetSourceSamples( padfSamples, pabySrc,
                                       nPixelBytes, nSampleFormat,
                                       nXSize, nYSize,
                                       nPixelGroupBytes,
                                       nPixelGroupBytes * nBlockXSize );
                dfTotal = 0;
                for( iSample = 0; iSample < nXSize*nYSize; iSample++ )
                {
                    dfTotal += padfSamples[iSample];
                }
                TIFF_SetSample( pabyDst, nPixelBytes, nSampleFormat, 
                                dfTotal / (nXSize*nYSize) );
                pabySrc += nOMult * nPixelGroupBytes;
                pabyDst += nPixelBytes;
            }
        }
    }
    free( padfSamples );
}

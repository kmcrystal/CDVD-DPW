static
void TIFF_DownSample_Subsampled( unsigned char *pabySrcTile, int nSample,
                                 uint32 nBlockXSize, uint32 nBlockYSize,
                                 unsigned char * pabyOTile,
                                 uint32 nOBlockXSize, uint32 nOBlockYSize,
                                 uint32 nTXOff, uint32 nTYOff, int nOMult,
                                 const char *pszResampling,
                                 int nHorSubsampling, int nVerSubsampling )
{
    
    int nSampleBlockSize;
    int nSourceSampleRowSize;
    int nDestSampleRowSize;
    uint32  nSourceX, nSourceY;
    uint32  nSourceXSec, nSourceYSec;
    uint32  nSourceXSecEnd, nSourceYSecEnd;
    uint32  nDestX, nDestY;
    int nSampleOffsetInSampleBlock;
    unsigned int nCummulator;
    unsigned int nCummulatorCount;
    nSampleBlockSize = nHorSubsampling * nVerSubsampling + 2;
    nSourceSampleRowSize = 
        ( ( nBlockXSize + nHorSubsampling - 1 ) / nHorSubsampling ) * nSampleBlockSize;
    nDestSampleRowSize = 
        ( ( nOBlockXSize + nHorSubsampling - 1 ) / nHorSubsampling ) * nSampleBlockSize;
    if( strncmp(pszResampling,"nearest",4) == 0
        || strncmp(pszResampling,"NEAR",4) == 0 )
    {
    	if( nSample == 0 )
        {
            for( nSourceY = 0, nDestY = nTYOff; 
                 nSourceY < nBlockYSize; 
                 nSourceY += nOMult, nDestY ++)
            {
                if( nDestY >= nOBlockYSize )
                    break;
                for( nSourceX = 0, nDestX = nTXOff; 
                     nSourceX < nBlockXSize; 
                     nSourceX += nOMult, nDestX ++)
                {
                    if( nDestX >= nOBlockXSize )
                        break;
                    * ( pabyOTile + ( nDestY / nVerSubsampling ) * nDestSampleRowSize
                        + ( nDestY % nVerSubsampling ) * nHorSubsampling
                        + ( nDestX / nHorSubsampling ) * nSampleBlockSize
                        + ( nDestX % nHorSubsampling ) ) =
                        * ( pabySrcTile + ( nSourceY / nVerSubsampling ) * nSourceSampleRowSize
                            + ( nSourceY % nVerSubsampling ) * nHorSubsampling
                            + ( nSourceX / nHorSubsampling ) * nSampleBlockSize
                            + ( nSourceX % nHorSubsampling ) );
                }
            }
        }
        else
        {
            nSampleOffsetInSampleBlock = nHorSubsampling * nVerSubsampling + nSample - 1;
            for( nSourceY = 0, nDestY = ( nTYOff / nVerSubsampling ); 
                 nSourceY < ( nBlockYSize / nVerSubsampling );
                 nSourceY += nOMult, nDestY ++)
            {
                if( nDestY*nVerSubsampling >= nOBlockYSize )
                    break;
            	for( nSourceX = 0, nDestX = ( nTXOff / nHorSubsampling ); 
                     nSourceX < ( nBlockXSize / nHorSubsampling );
                     nSourceX += nOMult, nDestX ++)
                {
                    if( nDestX*nHorSubsampling >= nOBlockXSize )
                        break;
                    * ( pabyOTile + nDestY * nDestSampleRowSize
                        + nDestX * nSampleBlockSize
                        + nSampleOffsetInSampleBlock ) =
                    	* ( pabySrcTile + nSourceY * nSourceSampleRowSize
                            + nSourceX * nSampleBlockSize
                            + nSampleOffsetInSampleBlock );
                }
            }
        }
    }
    else if( strncmp(pszResampling,"averag",6) == 0
             || strncmp(pszResampling,"AVERAG",6) == 0 )
    {
    	if( nSample == 0 )
        {
            for( nSourceY = 0, nDestY = nTYOff; nSourceY < nBlockYSize; nSourceY += nOMult, nDestY ++)
            {
                if( nDestY >= nOBlockYSize )
                    break;
                for( nSourceX = 0, nDestX = nTXOff; nSourceX < nBlockXSize; nSourceX += nOMult, nDestX ++)
                {
                    if( nDestX >= nOBlockXSize )
                        break;
                    nSourceXSecEnd = nSourceX + nOMult;
                    if( nSourceXSecEnd > nBlockXSize )
                        nSourceXSecEnd = nBlockXSize;
                    nSourceYSecEnd = nSourceY + nOMult;
                    if( nSourceYSecEnd > nBlockYSize )
                        nSourceYSecEnd = nBlockYSize;
                    nCummulator = 0;
                    for( nSourceYSec = nSourceY; nSourceYSec < nSourceYSecEnd; nSourceYSec ++)
                    {
                        for( nSourceXSec = nSourceX; nSourceXSec < nSourceXSecEnd; nSourceXSec ++)
                        {
                            nCummulator += * ( pabySrcTile + ( nSourceYSec / nVerSubsampling ) * nSourceSampleRowSize
                                               + ( nSourceYSec % nVerSubsampling ) * nHorSubsampling
                                               + ( nSourceXSec / nHorSubsampling ) * nSampleBlockSize
                                               + ( nSourceXSec % nHorSubsampling ) );
                        }
                    }
                    nCummulatorCount = ( nSourceXSecEnd - nSourceX ) * ( nSourceYSecEnd - nSourceY );
                    * ( pabyOTile + ( nDestY / nVerSubsampling ) * nDestSampleRowSize
                        + ( nDestY % nVerSubsampling ) * nHorSubsampling
                        + ( nDestX / nHorSubsampling ) * nSampleBlockSize
                        + ( nDestX % nHorSubsampling ) ) =
                        ( ( nCummulator + ( nCummulatorCount >> 1 ) ) / nCummulatorCount );
                }
            }
        }
        else
        {
            nSampleOffsetInSampleBlock = nHorSubsampling * nVerSubsampling + nSample - 1;
            for( nSourceY = 0, nDestY = ( nTYOff / nVerSubsampling ); nSourceY < ( nBlockYSize / nVerSubsampling );
                 nSourceY += nOMult, nDestY ++)
            {
                if( nDestY*nVerSubsampling >= nOBlockYSize )
                    break;
                for( nSourceX = 0, nDestX = ( nTXOff / nHorSubsampling ); nSourceX < ( nBlockXSize / nHorSubsampling );
                     nSourceX += nOMult, nDestX ++)
                {
                    if( nDestX*nHorSubsampling >= nOBlockXSize )
                        break;
                    nSourceXSecEnd = nSourceX + nOMult;
                    if( nSourceXSecEnd > ( nBlockXSize / nHorSubsampling ) )
                        nSourceXSecEnd = ( nBlockXSize / nHorSubsampling );
                    nSourceYSecEnd = nSourceY + nOMult;
                    if( nSourceYSecEnd > ( nBlockYSize / nVerSubsampling ) )
                        nSourceYSecEnd = ( nBlockYSize / nVerSubsampling );
                    nCummulator = 0;
                    for( nSourceYSec = nSourceY; nSourceYSec < nSourceYSecEnd; nSourceYSec ++)
                    {
                        for( nSourceXSec = nSourceX; nSourceXSec < nSourceXSecEnd; nSourceXSec ++)
                        {
                            nCummulator += * ( pabySrcTile + nSourceYSec * nSourceSampleRowSize
                                               + nSourceXSec * nSampleBlockSize
                                               + nSampleOffsetInSampleBlock );
                        }
                    }
                    nCummulatorCount = ( nSourceXSecEnd - nSourceX ) * ( nSourceYSecEnd - nSourceY );
                    * ( pabyOTile + nDestY * nDestSampleRowSize
                        + nDestX * nSampleBlockSize
                        + nSampleOffsetInSampleBlock ) =
                        ( ( nCummulator + ( nCummulatorCount >> 1 ) ) / nCummulatorCount );
                }
            }
        }
    }
}

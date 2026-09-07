static void 
TIFF_SetSample( unsigned char * pabyData, int nPixelBytes, int nSampleFormat, 
                double dfValue )
{
    if( nSampleFormat == SAMPLEFORMAT_UINT && nPixelBytes == 1 )
    {
        *pabyData = (unsigned char) MAX(0,MIN(255,dfValue));
    }
    else if( nSampleFormat == SAMPLEFORMAT_UINT && nPixelBytes == 2 )
    {
        *((uint16 *)pabyData) = (uint16) MAX(0,MIN(65535,dfValue));
    }
    else if( nSampleFormat == SAMPLEFORMAT_UINT && nPixelBytes == 4 )
    {
        *((uint32 *)pabyData) = (uint32) dfValue;
    }
    else if( nSampleFormat == SAMPLEFORMAT_INT && nPixelBytes == 2 )
    {
        *((int16 *)pabyData) = (int16) MAX(-32768,MIN(32767,dfValue));
    }
    else if( nSampleFormat == SAMPLEFORMAT_INT && nPixelBytes == 32 )
    {
        *((int32 *)pabyData) = (int32) dfValue;
    }
    else if( nSampleFormat == SAMPLEFORMAT_IEEEFP && nPixelBytes == 4 )
    {
        *((float *)pabyData) = (float) dfValue;
    }
    else if( nSampleFormat == SAMPLEFORMAT_IEEEFP && nPixelBytes == 8 )
    {
        *((double *)pabyData) = dfValue;
    }
}

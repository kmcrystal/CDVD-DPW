TIFFTagMethods *TIFFAccessTagMethods( TIFF *tif )
{
    return &(tif->tif_tagmethods);
}

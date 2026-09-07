int TIFFGetTagListCount( TIFF *tif )
{
    TIFFDirectory* td = &tif->tif_dir;
    return td->td_customValueCount;
}

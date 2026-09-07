int
TIFFCreateDirectory(TIFF* tif)
{
    TIFFDefaultDirectory(tif);
    tif->tif_diroff = 0;
    tif->tif_nextdiroff = 0;
    tif->tif_curoff = 0;
    tif->tif_row = (uint32) -1;
    tif->tif_curstrip = (tstrip_t) -1;
    return 0;
}

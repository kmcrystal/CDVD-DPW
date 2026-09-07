tdir_t
TIFFNumberOfDirectories(TIFF* tif)
{
    toff_t nextdir = tif->tif_header.tiff_diroff;
    tdir_t n = 0;
    while (nextdir != 0 && TIFFAdvanceDirectory(tif, &nextdir, NULL))
        n++;
    return (n);
}

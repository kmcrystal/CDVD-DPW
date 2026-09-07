int
TIFFReadImageIter(TIFF* tif,
    uint32 rwidth, uint32 rheight, uint8* raster, int stop)
{
    char emsg[1024];
    TIFFImageIter img;
    int ok;
    if (TIFFImageIterBegin(&img, tif, stop, emsg)) {
	
	ok = TIFFImageIterGet(&img, raster, rwidth, img.height);
	TIFFImageIterEnd(&img);
    } else {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), emsg);
	ok = 0;
    }
    return (ok);
}

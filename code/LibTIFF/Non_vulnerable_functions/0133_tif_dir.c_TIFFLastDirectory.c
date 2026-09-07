int
TIFFLastDirectory(TIFF* tif)
{
	return (tif->tif_nextdiroff == 0);
}

int
TIFFSetSubDirectory(TIFF* tif, uint32 diroff)
{
	tif->tif_nextdiroff = diroff;
	



	tif->tif_dirnumber = 0;
	return (TIFFReadDirectory(tif));
}

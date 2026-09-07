int
TIFFSetDirectory(TIFF* tif, tdir_t dirn)
{
	toff_t nextdir;
	tdir_t n;
	nextdir = tif->tif_header.tiff_diroff;
	for (n = dirn; n > 0 && nextdir != 0; n--)
		if (!TIFFAdvanceDirectory(tif, &nextdir, NULL))
			return (0);
	tif->tif_nextdiroff = nextdir;
	




	tif->tif_curdir = (dirn - n) - 1;
	



	tif->tif_dirnumber = 0;
	return (TIFFReadDirectory(tif));
}

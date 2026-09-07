int
TIFFUnlinkDirectory(TIFF* tif, tdir_t dirn)
{
	static const char module[] = "TIFFUnlinkDirectory";
	toff_t nextdir;
	toff_t off;
	tdir_t n;
	if (tif->tif_mode == O_RDONLY) {
		TIFFErrorExt(tif->tif_clientdata, module,
                             "Can not unlink directory in read-only file");
		return (0);
	}
	




	nextdir = tif->tif_header.tiff_diroff;
	off = sizeof (uint16) + sizeof (uint16);
	for (n = dirn-1; n > 0; n--) {
		if (nextdir == 0) {
			TIFFErrorExt(tif->tif_clientdata, module, "Directory %d does not exist", dirn);
			return (0);
		}
		if (!TIFFAdvanceDirectory(tif, &nextdir, &off))
			return (0);
	}
	



	if (!TIFFAdvanceDirectory(tif, &nextdir, NULL))
		return (0);
	




	(void) TIFFSeekFile(tif, off, SEEK_SET);
	if (tif->tif_flags & TIFF_SWAB)
		TIFFSwabLong(&nextdir);
	if (!WriteOK(tif, &nextdir, sizeof (uint32))) {
		TIFFErrorExt(tif->tif_clientdata, module, "Error writing directory link");
		return (0);
	}
	






	(*tif->tif_cleanup)(tif);
	if ((tif->tif_flags & TIFF_MYBUFFER) && tif->tif_rawdata) {
		_TIFFfree(tif->tif_rawdata);
		tif->tif_rawdata = NULL;
		tif->tif_rawcc = 0;
	}
	tif->tif_flags &= ~(TIFF_BEENWRITING|TIFF_BUFFERSETUP|TIFF_POSTENCODE);
	TIFFFreeDirectory(tif);
	TIFFDefaultDirectory(tif);
	tif->tif_diroff = 0;			
	tif->tif_nextdiroff = 0;		
	tif->tif_curoff = 0;
	tif->tif_row = (uint32) -1;
	tif->tif_curstrip = (tstrip_t) -1;
	return (1);
}

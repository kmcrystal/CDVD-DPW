static int
TIFFLinkDirectory(TIFF* tif)
{
	static const char module[] = "TIFFLinkDirectory";
	uint32 nextdir;
	uint32 diroff;
	tif->tif_diroff = (TIFFSeekFile(tif, (toff_t) 0, SEEK_END)+1) &~ 1;
	diroff = (uint32) tif->tif_diroff;
	if (tif->tif_flags & TIFF_SWAB)
		TIFFSwabLong(&diroff);
	if (tif->tif_flags & TIFF_INSUBIFD) {
		(void) TIFFSeekFile(tif, tif->tif_subifdoff, SEEK_SET);
		if (!WriteOK(tif, &diroff, sizeof (diroff))) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "%s: Error writing SubIFD directory link",
			    tif->tif_name);
			return (0);
		}
		




		if (--tif->tif_nsubifd)
			tif->tif_subifdoff += sizeof (diroff);
		else
			tif->tif_flags &= ~TIFF_INSUBIFD;
		return (1);
	}
	if (tif->tif_header.tiff_diroff == 0) {
		


		tif->tif_header.tiff_diroff = (uint32) tif->tif_diroff;
#define	HDROFF(f)	((toff_t) &(((TIFFHeader*) 0)->f))
		(void) TIFFSeekFile(tif, HDROFF(tiff_diroff), SEEK_SET);
		if (!WriteOK(tif, &diroff, sizeof (diroff))) {
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Error writing TIFF header");
			return (0);
		}
		return (1);
	}
	


	nextdir = tif->tif_header.tiff_diroff;
	do {
		uint16 dircount;
		if (!SeekOK(tif, nextdir) ||
		    !ReadOK(tif, &dircount, sizeof (dircount))) {
			TIFFErrorExt(tif->tif_clientdata, module, "Error fetching directory count");
			return (0);
		}
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabShort(&dircount);
		(void) TIFFSeekFile(tif,
		    dircount * sizeof (TIFFDirEntry), SEEK_CUR);
		if (!ReadOK(tif, &nextdir, sizeof (nextdir))) {
			TIFFErrorExt(tif->tif_clientdata, module, "Error fetching directory link");
			return (0);
		}
		if (tif->tif_flags & TIFF_SWAB)
			TIFFSwabLong(&nextdir);
	} while (nextdir != 0);
	(void) TIFFSeekFile(tif, -(toff_t) sizeof (nextdir), SEEK_CUR);
	if (!WriteOK(tif, &diroff, sizeof (diroff))) {
		TIFFErrorExt(tif->tif_clientdata, module, "Error writing directory link");
		return (0);
	}
	return (1);
}

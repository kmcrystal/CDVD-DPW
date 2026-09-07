TIFF*
TIFFFdOpen(int fd, const char* name, const char* mode)
{
	TIFF* tif;
	tif = TIFFClientOpen(name, mode,
	    (void*) fd,
	    _tiffReadProc, _tiffWriteProc, _tiffSeekProc, _tiffCloseProc,
	    _tiffSizeProc, _tiffMapProc, _tiffUnmapProc);
	if (tif)
		tif->tif_fd = fd;
	return (tif);
}

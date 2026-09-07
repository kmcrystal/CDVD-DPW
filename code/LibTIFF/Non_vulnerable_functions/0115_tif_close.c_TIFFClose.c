void
TIFFClose(TIFF* tif)
{
	TIFFCloseProc closeproc = tif->tif_closeproc;
	thandle_t fd = tif->tif_clientdata;
	TIFFCleanup(tif);
	(void) (*closeproc)(fd);
}

static uint64
t2pSeekFile(TIFF *tif, toff_t offset, int whence)
{
	thandle_t client = TIFFClientdata(tif);
	TIFFSeekProc proc = TIFFGetSeekProc(tif);
	if (proc)
		return proc(client, offset, whence);
	return -1;
}

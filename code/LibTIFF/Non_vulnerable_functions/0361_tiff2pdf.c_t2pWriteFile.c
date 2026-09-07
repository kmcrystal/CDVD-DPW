static tmsize_t 
t2pWriteFile(TIFF *tif, tdata_t data, tmsize_t size)
{
	thandle_t client = TIFFClientdata(tif);
	TIFFReadWriteProc proc = TIFFGetWriteProc(tif);
	if (proc)
		return proc(client, data, size);
	return -1;
}

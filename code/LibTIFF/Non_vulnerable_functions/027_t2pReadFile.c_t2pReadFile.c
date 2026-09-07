static tmsize_t 
t2pReadFile(TIFF *tif, tdata_t data, tmsize_t size)
{
	thandle_t client = TIFFClientdata(tif);
	TIFFReadWriteProc proc = TIFFGetReadProc(tif);
	if (proc)
		return proc(client, data, size);
	return -1;
}
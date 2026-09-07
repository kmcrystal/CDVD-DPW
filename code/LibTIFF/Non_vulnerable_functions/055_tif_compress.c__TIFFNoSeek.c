int
_TIFFNoSeek(TIFF* tif, uint32 off)
{
	(void) off;
	TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
		     "Compression algorithm does not support random access");
	return (0);
}

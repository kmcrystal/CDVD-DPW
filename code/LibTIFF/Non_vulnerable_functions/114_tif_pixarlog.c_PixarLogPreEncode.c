static int
PixarLogPreEncode(TIFF* tif, uint16 s)
{
	static const char module[] = "PixarLogPreEncode";
	PixarLogState *sp = EncoderState(tif);
	(void) s;
	assert(sp != NULL);
	sp->stream.next_out = tif->tif_rawdata;
	assert(sizeof(sp->stream.avail_out)==4);  



	sp->stream.avail_out = (uInt)tif->tif_rawdatasize;
	if ((tmsize_t)sp->stream.avail_out != tif->tif_rawdatasize)
	{
		TIFFErrorExt(tif->tif_clientdata, module, "ZLib cannot deal with buffers this size");
		return (0);
	}
	return (deflateReset(&sp->stream) == Z_OK);
}

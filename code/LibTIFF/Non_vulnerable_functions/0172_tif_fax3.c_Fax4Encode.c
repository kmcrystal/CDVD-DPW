static int
Fax4Encode(TIFF* tif, tidata_t bp, tsize_t cc, tsample_t s)
{
	Fax3CodecState *sp = EncoderState(tif);
	(void) s;
	while ((long)cc > 0) {
		if (!Fax3Encode2DRow(tif, bp, sp->refline, sp->b.rowpixels))
			return (0);
		_TIFFmemcpy(sp->refline, bp, sp->b.rowbytes);
		bp += sp->b.rowbytes;
		cc -= sp->b.rowbytes;
	}
	return (1);
}

static int
Fax3Encode(TIFF* tif, tidata_t bp, tsize_t cc, tsample_t s)
{
	Fax3CodecState* sp = EncoderState(tif);
	(void) s;
	while ((long)cc > 0) {
		if ((sp->b.mode & FAXMODE_NOEOL) == 0)
			Fax3PutEOL(tif);
		if (is2DEncoding(sp)) {
			if (sp->tag == G3_1D) {
				if (!Fax3Encode1DRow(tif, bp, sp->b.rowpixels))
					return (0);
				sp->tag = G3_2D;
			} else {
				if (!Fax3Encode2DRow(tif, bp, sp->refline,
                                                     sp->b.rowpixels))
					return (0);
				sp->k--;
			}
			if (sp->k == 0) {
				sp->tag = G3_1D;
				sp->k = sp->maxk-1;
			} else
				_TIFFmemcpy(sp->refline, bp, sp->b.rowbytes);
		} else {
			if (!Fax3Encode1DRow(tif, bp, sp->b.rowpixels))
				return (0);
		}
		bp += sp->b.rowbytes;
		cc -= sp->b.rowbytes;
	}
	return (1);
}

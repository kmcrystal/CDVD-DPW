static int
Fax3PreEncode(TIFF* tif, tsample_t s)
{
	Fax3CodecState* sp = EncoderState(tif);
	(void) s;
	assert(sp != NULL);
	sp->bit = 8;
	sp->data = 0;
	sp->tag = G3_1D;
	




	if (sp->refline)
		_TIFFmemset(sp->refline, 0x00, sp->b.rowbytes);
	if (is2DEncoding(sp)) {
		float res = tif->tif_dir.td_yresolution;
		









		if (tif->tif_dir.td_resolutionunit == RESUNIT_CENTIMETER)
			res *= 2.54f;		
		sp->maxk = (res > 150 ? 4 : 2);
		sp->k = sp->maxk-1;
	} else
		sp->k = sp->maxk = 0;
	return (1);
}

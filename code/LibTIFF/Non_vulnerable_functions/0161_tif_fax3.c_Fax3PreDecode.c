static int
Fax3PreDecode(TIFF* tif, tsample_t s)
{
	Fax3CodecState* sp = DecoderState(tif);
	(void) s;
	assert(sp != NULL);
	sp->bit = 0;			
	sp->data = 0;
	sp->EOLcnt = 0;			
	






	sp->bitmap =
	    TIFFGetBitRevTable(tif->tif_dir.td_fillorder != FILLORDER_LSB2MSB);
	if (sp->refruns) {		
		sp->refruns[0] = (uint32) sp->b.rowpixels;
		sp->refruns[1] = 0;
	}
	return (1);
}

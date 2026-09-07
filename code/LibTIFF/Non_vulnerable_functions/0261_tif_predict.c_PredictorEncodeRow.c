static int
PredictorEncodeRow(TIFF* tif, uint8* bp, tmsize_t cc, uint16 s)
{
	TIFFPredictorState *sp = PredictorState(tif);
	assert(sp != NULL);
	assert(sp->encodepfunc != NULL);
	assert(sp->encoderow != NULL);
	
	if( !(*sp->encodepfunc)(tif, bp, cc) )
        return 0;
	return (*sp->encoderow)(tif, bp, cc, s);
}

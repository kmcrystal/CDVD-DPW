static int
LZWSetupDecode(TIFF* tif)
{
	static const char module[] = "LZWSetupDecode";
	LZWCodecState* sp = DecoderState(tif);
	int code;
	if( sp == NULL )
	{
		



		tif->tif_data = (uint8*) _TIFFmalloc(sizeof(LZWCodecState));
		if (tif->tif_data == NULL)
		{
			TIFFErrorExt(tif->tif_clientdata, module, "No space for LZW state block");
			return (0);
		}
		DecoderState(tif)->dec_codetab = NULL;
		DecoderState(tif)->dec_decode = NULL;
		


		(void) TIFFPredictorInit(tif);
		sp = DecoderState(tif);
	}
	assert(sp != NULL);
	if (sp->dec_codetab == NULL) {
		sp->dec_codetab = (code_t*)_TIFFmalloc(CSIZE*sizeof (code_t));
		if (sp->dec_codetab == NULL) {
			TIFFErrorExt(tif->tif_clientdata, module,
				     "No space for LZW code table");
			return (0);
		}
		


		code = 255;
		do {
			sp->dec_codetab[code].value = (unsigned char)code;
			sp->dec_codetab[code].firstchar = (unsigned char)code;
			sp->dec_codetab[code].length = 1;
			sp->dec_codetab[code].next = NULL;
		} while (code--);
		


                 _TIFFmemset(&sp->dec_codetab[CODE_CLEAR], 0,
			     (CODE_FIRST - CODE_CLEAR) * sizeof (code_t));
	}
	return (1);
}

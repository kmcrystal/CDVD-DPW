int
TIFFInitLZW(TIFF* tif, int scheme)
{
	static const char module[] = "TIFFInitLZW";
	assert(scheme == COMPRESSION_LZW);
	


	tif->tif_data = (uint8*) _TIFFmalloc(sizeof (LZWCodecState));
	if (tif->tif_data == NULL)
		goto bad;
	DecoderState(tif)->dec_codetab = NULL;
	DecoderState(tif)->dec_decode = NULL;
	EncoderState(tif)->enc_hashtab = NULL;
        LZWState(tif)->rw_mode = tif->tif_mode;
	


	tif->tif_fixuptags = LZWFixupTags; 
	tif->tif_setupdecode = LZWSetupDecode;
	tif->tif_predecode = LZWPreDecode;
	tif->tif_decoderow = LZWDecode;
	tif->tif_decodestrip = LZWDecode;
	tif->tif_decodetile = LZWDecode;
	tif->tif_setupencode = LZWSetupEncode;
	tif->tif_preencode = LZWPreEncode;
	tif->tif_postencode = LZWPostEncode;
	tif->tif_encoderow = LZWEncode;
	tif->tif_encodestrip = LZWEncode;
	tif->tif_encodetile = LZWEncode;
	tif->tif_cleanup = LZWCleanup;
	


	(void) TIFFPredictorInit(tif);
	return (1);
bad:
	TIFFErrorExt(tif->tif_clientdata, module, 
		     "No space for LZW state block");
	return (0);
}

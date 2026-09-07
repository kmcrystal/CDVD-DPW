static void
LZWCleanup(TIFF* tif)
{
	(void)TIFFPredictorCleanup(tif);
	assert(tif->tif_data != 0);
	if (DecoderState(tif)->dec_codetab)
		_TIFFfree(DecoderState(tif)->dec_codetab);
	if (EncoderState(tif)->enc_hashtab)
		_TIFFfree(EncoderState(tif)->enc_hashtab);
	_TIFFfree(tif->tif_data);
	tif->tif_data = NULL;
	_TIFFSetDefaultCompressionState(tif);
}

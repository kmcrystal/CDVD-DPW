static void
Fax3Cleanup(TIFF* tif)
{
	Fax3CodecState* sp = DecoderState(tif);
	assert(sp != 0);
	tif->tif_tagmethods.vgetfield = sp->b.vgetparent;
	tif->tif_tagmethods.vsetfield = sp->b.vsetparent;
	if (sp->runs)
		_TIFFfree(sp->runs);
	if (sp->refline)
		_TIFFfree(sp->refline);
	if (Fax3State(tif)->subaddress)
		_TIFFfree(Fax3State(tif)->subaddress);
	_TIFFfree(tif->tif_data);
	tif->tif_data = NULL;
	_TIFFSetDefaultCompressionState(tif);
}

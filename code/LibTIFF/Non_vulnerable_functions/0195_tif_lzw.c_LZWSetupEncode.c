static int
LZWSetupEncode(TIFF* tif)
{
	static const char module[] = "LZWSetupEncode";
	LZWCodecState* sp = EncoderState(tif);
	assert(sp != NULL);
	sp->enc_hashtab = (hash_t*) _TIFFmalloc(HSIZE*sizeof (hash_t));
	if (sp->enc_hashtab == NULL) {
		TIFFErrorExt(tif->tif_clientdata, module,
			     "No space for LZW hash table");
		return (0);
	}
	return (1);
}

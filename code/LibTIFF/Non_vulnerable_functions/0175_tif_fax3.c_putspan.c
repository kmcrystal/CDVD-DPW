static void
putspan(TIFF* tif, int32 span, const tableentry* tab)
{
	Fax3CodecState* sp = EncoderState(tif);
	unsigned int bit = sp->bit;
	int data = sp->data;
	unsigned int code, length;
	while (span >= 2624) {
		const tableentry* te = &tab[63 + (2560>>6)];
		code = te->code, length = te->length;
#ifdef FAX3_DEBUG
		DEBUG_PRINT("MakeUp", te->runlen);
#endif
		_PutBits(tif, code, length);
		span -= te->runlen;
	}
	if (span >= 64) {
		const tableentry* te = &tab[63 + (span>>6)];
		assert(te->runlen == 64*(span>>6));
		code = te->code, length = te->length;
#ifdef FAX3_DEBUG
		DEBUG_PRINT("MakeUp", te->runlen);
#endif
		_PutBits(tif, code, length);
		span -= te->runlen;
	}
	code = tab[span].code, length = tab[span].length;
#ifdef FAX3_DEBUG
	DEBUG_PRINT("  Term", tab[span].runlen);
#endif
	_PutBits(tif, code, length);
	sp->data = data;
	sp->bit = bit;
}

static int
Fax3Encode1DRow(TIFF* tif, unsigned char* bp, uint32 bits)
{
	Fax3CodecState* sp = EncoderState(tif);
	int32 span;
        uint32 bs = 0;
	for (;;) {
		span = find0span(bp, bs, bits);		
		putspan(tif, span, TIFFFaxWhiteCodes);
		bs += span;
		if (bs >= bits)
			break;
		span = find1span(bp, bs, bits);		
		putspan(tif, span, TIFFFaxBlackCodes);
		bs += span;
		if (bs >= bits)
			break;
	}
	if (sp->b.mode & (FAXMODE_BYTEALIGN|FAXMODE_WORDALIGN)) {
		if (sp->bit != 8)			
			Fax3FlushBits(tif, sp);
		if ((sp->b.mode&FAXMODE_WORDALIGN) &&
		    !isAligned(tif->tif_rawcp, uint16))
			Fax3FlushBits(tif, sp);
	}
	return (1);
}

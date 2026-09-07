static void
Fax3Close(TIFF* tif)
{
	if ((Fax3State(tif)->mode & FAXMODE_NORTC) == 0) {
		Fax3CodecState* sp = EncoderState(tif);
		unsigned int code = EOL;
		unsigned int length = 12;
		int i;
		if (is2DEncoding(sp))
			code = (code<<1) | (sp->tag == G3_1D), length++;
		for (i = 0; i < 6; i++)
			Fax3PutBits(tif, code, length);
		Fax3FlushBits(tif, sp);
	}
}

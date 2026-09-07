static void
Fax3PutEOL(TIFF* tif)
{
	Fax3CodecState* sp = EncoderState(tif);
	unsigned int bit = sp->bit;
	int data = sp->data;
	unsigned int code, length, tparm;
	if (sp->b.groupoptions & GROUP3OPT_FILLBITS) {
		




		int align = 8 - 4;
		if (align != sp->bit) {
			if (align > sp->bit)
				align = sp->bit + (8 - align);
			else
				align = sp->bit - align;
			code = 0;
			tparm=align; 
			_PutBits(tif, 0, tparm);
		}
	}
	code = EOL, length = 12;
	if (is2DEncoding(sp))
		code = (code<<1) | (sp->tag == G3_1D), length++;
	_PutBits(tif, code, length);
	sp->data = data;
	sp->bit = bit;
}

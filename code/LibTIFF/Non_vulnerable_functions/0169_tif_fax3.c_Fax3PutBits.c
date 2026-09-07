static void
Fax3PutBits(TIFF* tif, unsigned int bits, unsigned int length)
{
	Fax3CodecState* sp = EncoderState(tif);
	unsigned int bit = sp->bit;
	int data = sp->data;
	_PutBits(tif, bits, length);
	sp->data = data;
	sp->bit = bit;
}

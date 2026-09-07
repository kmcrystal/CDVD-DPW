static int
Fax3PostEncode(TIFF* tif)
{
	Fax3CodecState* sp = EncoderState(tif);
	if (sp->bit != 8)
		Fax3FlushBits(tif, sp);
	return (1);
}

static int
Fax4PostEncode(TIFF* tif)
{
	Fax3CodecState *sp = EncoderState(tif);
	
	Fax3PutBits(tif, EOL, 12);
	Fax3PutBits(tif, EOL, 12);
	if (sp->bit != 8)
		Fax3FlushBits(tif, sp);
	return (1);
}

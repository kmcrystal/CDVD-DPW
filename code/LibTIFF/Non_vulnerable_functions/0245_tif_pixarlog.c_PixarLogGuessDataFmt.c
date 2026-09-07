static int
PixarLogGuessDataFmt(TIFFDirectory *td)
{
	int guess = PIXARLOGDATAFMT_UNKNOWN;
	int format = td->td_sampleformat;
	


	switch (td->td_bitspersample) {
	 case 32:
		if (format == SAMPLEFORMAT_IEEEFP)
			guess = PIXARLOGDATAFMT_FLOAT;
		break;
	 case 16:
		if (format == SAMPLEFORMAT_VOID || format == SAMPLEFORMAT_UINT)
			guess = PIXARLOGDATAFMT_16BIT;
		break;
	 case 12:
		if (format == SAMPLEFORMAT_VOID || format == SAMPLEFORMAT_INT)
			guess = PIXARLOGDATAFMT_12BITPICIO;
		break;
	 case 11:
		if (format == SAMPLEFORMAT_VOID || format == SAMPLEFORMAT_UINT)
			guess = PIXARLOGDATAFMT_11BITLOG;
		break;
	 case 8:
		if (format == SAMPLEFORMAT_VOID || format == SAMPLEFORMAT_UINT)
			guess = PIXARLOGDATAFMT_8BIT;
		break;
	}
	return guess;
}

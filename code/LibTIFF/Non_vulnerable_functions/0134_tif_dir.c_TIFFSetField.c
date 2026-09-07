int
TIFFSetField(TIFF* tif, ttag_t tag, ...)
{
	va_list ap;
	int status;
	va_start(ap, tag);
	status = TIFFVSetField(tif, tag, ap);
	va_end(ap);
	return (status);
}

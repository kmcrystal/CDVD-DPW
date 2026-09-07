int
TIFFGetField(TIFF* tif, ttag_t tag, ...)
{
	int status;
	va_list ap;
	va_start(ap, tag);
	status = TIFFVGetField(tif, tag, ap);
	va_end(ap);
	return (status);
}

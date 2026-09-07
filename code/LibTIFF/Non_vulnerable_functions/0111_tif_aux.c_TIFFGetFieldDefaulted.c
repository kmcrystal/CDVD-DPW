int
TIFFGetFieldDefaulted(TIFF* tif, uint32 tag, ...)
{
	int ok;
	va_list ap;
	va_start(ap, tag);
	ok =  TIFFVGetFieldDefaulted(tif, tag, ap);
	va_end(ap);
	return (ok);
}

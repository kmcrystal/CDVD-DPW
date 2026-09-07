static void
tiffinfo(TIFF* tif, uint16 order, long flags)
{
	TIFFPrintDirectory(tif, stdout, flags);
	if (!readdata)
		return;
	if (rawdata) {
		if (order) {
			uint16 o;
			TIFFGetFieldDefaulted(tif,
			    TIFFTAG_FILLORDER, &o);
			TIFFReadRawData(tif, o != order);
		} else
			TIFFReadRawData(tif, 0);
	} else {
		if (order)
			TIFFSetField(tif, TIFFTAG_FILLORDER, order);
		TIFFReadData(tif);
	}
}

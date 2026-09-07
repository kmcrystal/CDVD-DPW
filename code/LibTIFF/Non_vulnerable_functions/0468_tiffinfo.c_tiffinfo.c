static void
tiffinfo(TIFF* tif, uint16 order, long flags, int is_image)
{
	TIFFPrintDirectory(tif, stdout, flags);
	if (!readdata || !is_image)
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

int
write_scanlines(TIFF *tif, const tdata_t array, const tsize_t size)
{
	uint32		length, row;
	tsize_t		scanlinesize, offset;
        (void) size;
	if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length)) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_IMAGELENGTH);
		return -1;
	}
	scanlinesize = TIFFScanlineSize(tif);
	if (!scanlinesize) {
		fprintf (stderr, "Wrong size of scanline.\n");
		return -1;
	}
	for (offset = 0, row = 0; row < length; offset+=scanlinesize, row++) {
		if (TIFFWriteScanline(tif, (char *)array + offset, row, 0) == -1) {
			fprintf (stderr,
				 "Can't write image data at row %lu.\n", (long) row);
			return -1;
		}
        }
	return 0;
}

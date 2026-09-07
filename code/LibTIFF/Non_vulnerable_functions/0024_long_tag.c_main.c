int
main(int argc, char **argv)
{
	TIFF		*tif;
	unsigned int	i;
	unsigned char	buf[3] = { 0, 127, 255 };
        (void) argc;
        (void) argv;
	
	tif = TIFFOpen(filename, "w");
	if (!tif) {
		fprintf (stderr, "Can't create test TIFF file %s.\n", filename);
		return 1;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width)) {
		fprintf (stderr, "Can't set ImageWidth tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length)) {
		fprintf (stderr, "Can't set ImageLength tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8)) {
		fprintf (stderr, "Can't set BitsPerSample tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) {
		fprintf (stderr, "Can't set PlanarConfiguration tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB)) {
		fprintf (stderr, "Can't set PhotometricInterpretation tag.\n");
		goto failure;
	}
	for (i = 0; i < NTAGS; i++) {
		if (!TIFFSetField(tif, long_tags[i].tag,
				  long_tags[i].value)) {
			fprintf(stderr, "Can't set tag %d.\n",
				(int)long_tags[i].tag);
			goto failure;
		}
	}
	
	if (TIFFWriteScanline(tif, buf, 0, 0) == -1) {
		fprintf (stderr, "Can't write image data.\n");
		goto failure;
	}
	TIFFClose(tif);
	
	tif = TIFFOpen(filename, "r");
	if (!tif) {
		fprintf (stderr, "Can't open test TIFF file %s.\n", filename);
		return 1;
	}
	if (CheckLongField(tif, TIFFTAG_IMAGEWIDTH, width) < 0)
		goto failure;
	if (CheckLongField(tif, TIFFTAG_IMAGELENGTH, length) < 0)
		goto failure;
	if (CheckLongField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip) < 0)
		goto failure;
	for (i = 0; i < NTAGS; i++) {
		if (CheckLongField(tif, long_tags[i].tag,
				   long_tags[i].value) < 0)
			goto failure;
	}
	TIFFClose(tif);
	
	unlink(filename);
	return 0;
failure:
	
	TIFFClose(tif);
	unlink(filename);
	return 1;
}

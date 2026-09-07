int
main()
{
	TIFF		*tif;
	size_t		i;
	unsigned char	buf[SPP] = { 0, 127, 255 };
	
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
	if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps)) {
		fprintf (stderr, "Can't set BitsPerSample tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, SPP)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig)) {
		fprintf (stderr, "Can't set PlanarConfiguration tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric)) {
		fprintf (stderr, "Can't set PhotometricInterpretation tag.\n");
		goto failure;
	}
	for (i = 0; i < NSINGLETAGS; i++) {
		if (!TIFFSetField(tif, short_single_tags[i].tag,
				  short_single_tags[i].value)) {
			fprintf(stderr, "Can't set tag %lu.\n",
				(unsigned long)short_single_tags[i].tag);
			goto failure;
		}
	}
	for (i = 0; i < NPAIREDTAGS; i++) {
		if (!TIFFSetField(tif, short_paired_tags[i].tag,
				  short_paired_tags[i].values[0],
				  short_paired_tags[i].values[1])) {
			fprintf(stderr, "Can't set tag %lu.\n",
				(unsigned long)short_paired_tags[i].tag);
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
	if (CheckShortField(tif, TIFFTAG_BITSPERSAMPLE, bps) < 0)
		goto failure;
	if (CheckShortField(tif, TIFFTAG_PHOTOMETRIC, photometric) < 0)
		goto failure;
	if (CheckShortField(tif, TIFFTAG_SAMPLESPERPIXEL, SPP) < 0)
		goto failure;
	if (CheckLongField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip) < 0)
		goto failure;
	if (CheckShortField(tif, TIFFTAG_PLANARCONFIG, planarconfig) < 0)
		goto failure;
	for (i = 0; i < NSINGLETAGS; i++) {
		if (CheckShortField(tif, short_single_tags[i].tag,
				    short_single_tags[i].value) < 0)
			goto failure;
	}
	for (i = 0; i < NPAIREDTAGS; i++) {
		if (CheckShortPairedField(tif, short_paired_tags[i].tag,
					  short_paired_tags[i].values) < 0)
			goto failure;
	}
	TIFFClose(tif);
	
	unlink(filename);
	return 0;
failure:
	



	TIFFClose(tif);
	return 1;
}

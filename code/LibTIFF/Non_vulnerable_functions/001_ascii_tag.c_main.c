int
main()
{
	TIFF		*tif;
	size_t		i;
	unsigned char	buf[] = { 0, 127, 255 };
	char		*value;
	
	tif = TIFFOpen(filename, "w");
	if (!tif) {
		fprintf (stderr, "Can't create test TIFF file %s.\n", filename);
		return 1;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1)) {
		fprintf (stderr, "Can't set ImageWidth tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1)) {
		fprintf (stderr, "Can't set ImageLength tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8)) {
		fprintf (stderr, "Can't set BitsPerSample tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, sizeof(buf))) {
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
		if (!TIFFSetField(tif, ascii_tags[i].tag,
				  ascii_tags[i].value)) {
			fprintf(stderr, "Can't set tag %lu.\n",
				(unsigned long)ascii_tags[i].tag);
			goto failure;
		}
	}
	
	if (!TIFFSetField(tif, TIFFTAG_NUMBEROFINKS, 3)) {
		fprintf (stderr, "Can't set tag %d (NUMBEROFINKS).\n",
                         TIFFTAG_NUMBEROFINKS);
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_INKNAMES, ink_names_size, ink_names)) {
		fprintf (stderr, "Can't set tag %d (INKNAMES).\n",
			 TIFFTAG_INKNAMES);
		goto failure;
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
	for (i = 0; i < NTAGS; i++) {
		if (!TIFFGetField(tif, ascii_tags[i].tag, &value)
		    || strcmp(value, ascii_tags[i].value)) {
			fprintf(stderr, "Can't get tag %lu.\n",
				(unsigned long)ascii_tags[i].tag);
			goto failure;
		}
	}
	if (!TIFFGetField(tif, TIFFTAG_INKNAMES, &value)
	    || memcmp(value, ink_names, ink_names_size)) {
		fprintf (stderr, "Can't get tag %d (INKNAMES).\n",
			 TIFFTAG_INKNAMES);
		goto failure;
	}
	TIFFClose(tif);
	
	unlink(filename);
	return 0;
failure:
	



	TIFFClose(tif);
	return 1;
}

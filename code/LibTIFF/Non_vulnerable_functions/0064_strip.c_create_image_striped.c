int
create_image_striped(const char *name, uint32 width, uint32 length,
		      uint32 rowsperstrip, uint16 compression,
		      uint16 spp, uint16 bps, uint16 photometric,
		      uint16 sampleformat, uint16 planarconfig,
		      const tdata_t array, const tsize_t size)
{
	TIFF		*tif;
	
	tif = TIFFOpen(name, "w");
	if (!tif)
		goto openfailure;
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
	if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp)) {
		fprintf (stderr, "Can't set SamplesPerPixel tag.\n");
		goto failure;
	}
	if (!TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip)) {
		fprintf (stderr, "Can't set RowsPerStrip tag.\n");
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
	if (write_strips(tif, array, size) < 0) {
		fprintf (stderr, "Can't write image data.\n");
		goto failure;
	}
	TIFFClose(tif);
	return 0;
failure:
	TIFFClose(tif);
openfailure:
	fprintf (stderr, "Can't create test TIFF file %s:\n"
"    ImageWidth=%ld, ImageLength=%ld, RowsPerStrip=%ld, Compression=%d,\n"
"    BitsPerSample=%d, SamplesPerPixel=%d, SampleFormat=%d,\n"
"    PlanarConfiguration=%d, PhotometricInterpretation=%d.\n",
		 name, (long) width, (long) length, (long) rowsperstrip,
                 compression, bps, spp, sampleformat, planarconfig,
		 photometric);
	return -1;
}

int
read_image_striped(const char *name, uint32 width, uint32 length,
		    uint32 rowsperstrip, uint16 compression,
		    uint16 spp, uint16 bps, uint16 photometric,
		    uint16 sampleformat, uint16 planarconfig,
		    const tdata_t array, const tsize_t size)
{
	TIFF		*tif;
	uint16		value_u16;
	uint32		value_u32;
	
	tif = TIFFOpen(name, "r");
	if (!tif)
		goto openfailure;
	if (TIFFIsTiled(tif)) {
		fprintf (stderr, "Can't read image %s, it is tiled.\n",
			 name);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &value_u32)
	    || value_u32 != width) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_IMAGEWIDTH);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &value_u32)
	    || value_u32 != length) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_IMAGELENGTH);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &value_u16)
	    || value_u16 != bps) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_BITSPERSAMPLE);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &value_u16)
	    || value_u16 != photometric) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_PHOTOMETRIC);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &value_u16)
	    || value_u16 != spp) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_SAMPLESPERPIXEL);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &value_u32)
	    || value_u32 != rowsperstrip) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_ROWSPERSTRIP);
		goto failure;
	}
	if (!TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &value_u16)
	    || value_u16 != planarconfig) {
		fprintf (stderr, "Can't get tag %d.\n", TIFFTAG_PLANARCONFIG);
		goto failure;
	}
	if (read_strips(tif, array, size) < 0) {
		fprintf (stderr, "Can't read image data.\n");
		goto failure;
	}
	TIFFClose(tif);
	return 0;
failure:
	TIFFClose(tif);
openfailure:
	fprintf (stderr, "Can't read test TIFF file %s:\n"
"    ImageWidth=%ld, ImageLength=%ld, RowsPerStrip=%ld, Compression=%d,\n"
"    BitsPerSample=%d, SamplesPerPixel=%d, SampleFormat=%d,\n"
"    PlanarConfiguration=%d, PhotometricInterpretation=%d.\n",
		 name, (long) width, (long) length, (long) rowsperstrip,
                 compression, bps, spp, sampleformat, planarconfig,
		 photometric);
	return -1;
}

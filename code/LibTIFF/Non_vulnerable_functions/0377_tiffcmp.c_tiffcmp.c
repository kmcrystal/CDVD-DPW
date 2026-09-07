static int
tiffcmp(TIFF* tif1, TIFF* tif2)
{
	uint16 config1, config2;
	tsize_t size1;
	uint32 row;
	tsample_t s;
	unsigned char *buf1, *buf2;
	if (!CheckShortTag(tif1, tif2, TIFFTAG_BITSPERSAMPLE, "BitsPerSample"))
		return (0);
	if (!CheckShortTag(tif1, tif2, TIFFTAG_SAMPLESPERPIXEL, "SamplesPerPixel"))
		return (0);
	if (!CheckLongTag(tif1, tif2, TIFFTAG_IMAGEWIDTH, "ImageWidth"))
		return (0);
	if (!cmptags(tif1, tif2))
		return (1);
	(void) TIFFGetField(tif1, TIFFTAG_BITSPERSAMPLE, &bitspersample);
	(void) TIFFGetField(tif1, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
	(void) TIFFGetField(tif1, TIFFTAG_SAMPLEFORMAT, &sampleformat);
	(void) TIFFGetField(tif1, TIFFTAG_IMAGEWIDTH, &imagewidth);
	(void) TIFFGetField(tif1, TIFFTAG_IMAGELENGTH, &imagelength);
	(void) TIFFGetField(tif1, TIFFTAG_PLANARCONFIG, &config1);
	(void) TIFFGetField(tif2, TIFFTAG_PLANARCONFIG, &config2);
	buf1 = (unsigned char *)_TIFFmalloc(size1 = TIFFScanlineSize(tif1));
	buf2 = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif2));
	if (buf1 == NULL || buf2 == NULL) {
		fprintf(stderr, "No space for scanline buffers\n");
		exit(-1);
	}
	if (config1 != config2 && bitspersample != 8 && samplesperpixel > 1) {
		fprintf(stderr,
"Can't handle different planar configuration w/ different bits/sample\n");
		goto bad;
	}
#define	pack(a,b)	((a)<<8)|(b)
	switch (pack(config1, config2)) {
	case pack(PLANARCONFIG_SEPARATE, PLANARCONFIG_CONTIG):
		for (row = 0; row < imagelength; row++) {
			if (TIFFReadScanline(tif2, buf2, row, 0) < 0)
				checkEOF(tif2, row, -1)
			for (s = 0; s < samplesperpixel; s++) {
				if (TIFFReadScanline(tif1, buf1, row, s) < 0)
					checkEOF(tif1, row, s)
				if (SeparateCompare(1, s, row, buf2, buf1) < 0)
					goto bad1;
			}
		}
		break;
	case pack(PLANARCONFIG_CONTIG, PLANARCONFIG_SEPARATE):
		for (row = 0; row < imagelength; row++) {
			if (TIFFReadScanline(tif1, buf1, row, 0) < 0)
				checkEOF(tif1, row, -1)
			for (s = 0; s < samplesperpixel; s++) {
				if (TIFFReadScanline(tif2, buf2, row, s) < 0)
					checkEOF(tif2, row, s)
				if (SeparateCompare(0, s, row, buf1, buf2) < 0)
					goto bad1;
			}
		}
		break;
	case pack(PLANARCONFIG_SEPARATE, PLANARCONFIG_SEPARATE):
		for (s = 0; s < samplesperpixel; s++)
			for (row = 0; row < imagelength; row++) {
				if (TIFFReadScanline(tif1, buf1, row, s) < 0)
					checkEOF(tif1, row, s)
				if (TIFFReadScanline(tif2, buf2, row, s) < 0)
					checkEOF(tif2, row, s)
				if (ContigCompare(s, row, buf1, buf2, size1) < 0)
					goto bad1;
			}
		break;
	case pack(PLANARCONFIG_CONTIG, PLANARCONFIG_CONTIG):
		for (row = 0; row < imagelength; row++) {
			if (TIFFReadScanline(tif1, buf1, row, 0) < 0)
				checkEOF(tif1, row, -1)
			if (TIFFReadScanline(tif2, buf2, row, 0) < 0)
				checkEOF(tif2, row, -1)
			if (ContigCompare(-1, row, buf1, buf2, size1) < 0)
				goto bad1;
		}
		break;
	}
	if (buf1) _TIFFfree(buf1);
	if (buf2) _TIFFfree(buf2);
	return (1);
bad:
	if (stopondiff)
		exit(1);
bad1:
	if (buf1) _TIFFfree(buf1);
	if (buf2) _TIFFfree(buf2);
	return (0);
}

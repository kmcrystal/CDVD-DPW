int
main(int argc, char* argv[])
{
	TIFF *in, *out;
	uint16 samplesperpixel, bitspersample = 1, shortv;
	float floatv;
	char thing[1024];
	uint32 rowsperstrip = (uint32) -1;
	uint16 fillorder = 0;
	int c;
#if !HAVE_DECL_OPTARG
	extern int optind;
	extern char *optarg;
#endif
	while ((c = getopt(argc, argv, "c:f:r:t:")) != -1)
		switch (c) {
		case 'c':		
			if (!processCompressOptions(optarg))
				usage();
			break;
		case 'f':		
			if (streq(optarg, "lsb2msb"))
				fillorder = FILLORDER_LSB2MSB;
			else if (streq(optarg, "msb2lsb"))
				fillorder = FILLORDER_MSB2LSB;
			else
				usage();
			break;
		case 'r':		
			rowsperstrip = atoi(optarg);
			break;
		case 't':
			threshold = atoi(optarg);
			if (threshold < 0)
				threshold = 0;
			else if (threshold > 255)
				threshold = 255;
			break;
		case '?':
			usage();
			
		}
	if (argc - optind < 2)
		usage();
	in = TIFFOpen(argv[optind], "r");
	if (in == NULL)
		return (-1);
	TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
	if (samplesperpixel != 1) {
		fprintf(stderr, "%s: Not a b&w image.\n", argv[0]);
		return (-1);
	}
	TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
	if (bitspersample != 8) {
		fprintf(stderr,
		    " %s: Sorry, only handle 8-bit samples.\n", argv[0]);
		return (-1);
	}
	out = TIFFOpen(argv[optind+1], "w");
	if (out == NULL)
		return (-1);
	CopyField(TIFFTAG_IMAGEWIDTH, imagewidth);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &imagelength);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, imagelength-1);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 1);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
	if (fillorder)
		TIFFSetField(out, TIFFTAG_FILLORDER, fillorder);
	else
		CopyField(TIFFTAG_FILLORDER, shortv);
	snprintf(thing, sizeof(thing), "Dithered B&W version of %s", argv[optind]);
	TIFFSetField(out, TIFFTAG_IMAGEDESCRIPTION, thing);
	CopyField(TIFFTAG_PHOTOMETRIC, shortv);
	CopyField(TIFFTAG_ORIENTATION, shortv);
	CopyField(TIFFTAG_XRESOLUTION, floatv);
	CopyField(TIFFTAG_YRESOLUTION, floatv);
	CopyField(TIFFTAG_RESOLUTIONUNIT, shortv);
        rowsperstrip = TIFFDefaultStripSize(out, rowsperstrip);
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	switch (compression) {
	case COMPRESSION_CCITTFAX3:
		TIFFSetField(out, TIFFTAG_GROUP3OPTIONS, group3options);
		break;
	case COMPRESSION_LZW:
	case COMPRESSION_DEFLATE:
		if (predictor)
			TIFFSetField(out, TIFFTAG_PREDICTOR, predictor);
		break;
	}
	fsdither(in, out);
	TIFFClose(in);
	TIFFClose(out);
	return (0);
}

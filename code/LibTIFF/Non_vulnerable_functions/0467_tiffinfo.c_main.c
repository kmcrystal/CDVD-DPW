int
main(int argc, char* argv[])
{
	int dirnum = -1, multiplefiles, c;
	uint16 order = 0;
	TIFF* tif;
	extern int optind;
	extern char* optarg;
	long flags = 0;
	uint64 diroff = 0;
	int chopstrips = 0;		
	while ((c = getopt(argc, argv, "f:o:cdDSjilmrsvwz0123456789")) != -1)
		switch (c) {
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			dirnum = atoi(&argv[optind-1][1]);
			break;
		case 'd':
			showdata++;
			
		case 'D':
			readdata++;
			break;
		case 'c':
			flags |= TIFFPRINT_COLORMAP | TIFFPRINT_CURVES;
			break;
		case 'f':		
			if (streq(optarg, "lsb2msb"))
				order = FILLORDER_LSB2MSB;
			else if (streq(optarg, "msb2lsb"))
				order = FILLORDER_MSB2LSB;
			else
				usage();
			break;
		case 'i':
			stoponerr = 0;
			break;
		case 'o':
			diroff = strtoul(optarg, NULL, 0);
			break;
		case 'j':
			flags |= TIFFPRINT_JPEGQTABLES |
				 TIFFPRINT_JPEGACTABLES |
				 TIFFPRINT_JPEGDCTABLES;
			break;
		case 'r':
			rawdata = 1;
			break;
		case 's':
			flags |= TIFFPRINT_STRIPS;
			break;
		case 'w':
			showwords = 1;
			break;
		case 'z':
			chopstrips = 1;
			break;
		case '?':
			usage();
			
		}
	if (optind >= argc)
		usage();
	old_error_handler = TIFFSetErrorHandler(PrivateErrorHandler);
	multiplefiles = (argc - optind > 1);
	for (; optind < argc; optind++) {
		if (multiplefiles)
			printf("%s:\n", argv[optind]);
		tif = TIFFOpen(argv[optind], chopstrips ? "rC" : "rc");
		if (tif != NULL) {
			if (dirnum != -1) {
				if (TIFFSetDirectory(tif, (tdir_t) dirnum))
					tiffinfo(tif, order, flags, 1);
			} else if (diroff != 0) {
				if (TIFFSetSubDirectory(tif, diroff))
					tiffinfo(tif, order, flags, 1);
			} else {
				do {
					toff_t offset;
					tiffinfo(tif, order, flags, 1);
					if (TIFFGetField(tif, TIFFTAG_EXIFIFD,
							 &offset)) {
						if (TIFFReadEXIFDirectory(tif, offset)) {
							tiffinfo(tif, order, flags, 0);
						}
					}
				} while (TIFFReadDirectory(tif));
			}
			TIFFClose(tif);
		}
	}
	return (status);
}

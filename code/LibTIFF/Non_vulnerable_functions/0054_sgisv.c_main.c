int
main(int argc, char* argv[])
{
	int c;
#if !HAVE_DECL_OPTARG
	extern int optind;
	extern char* optarg;
#endif
	while ((c = getopt(argc, argv, "c:p:r:")) != -1)
		switch (c) {
		case 'b':		
			photometric = PHOTOMETRIC_MINISBLACK;
			break;
		case 'c':		
			if (streq(optarg, "none"))
				compression = COMPRESSION_NONE;
			else if (streq(optarg, "packbits"))
				compression = COMPRESSION_PACKBITS;
			else if (strneq(optarg, "jpeg", 4)) {
				char* cp = strchr(optarg, ':');
				if (cp && isdigit(cp[1]))
					quality = atoi(cp+1);
				if (cp && strchr(cp, 'r'))
					jpegcolormode = JPEGCOLORMODE_RAW;
				compression = COMPRESSION_JPEG;
			} else if (strneq(optarg, "lzw", 3)) {
				char* cp = strchr(optarg, ':');
				if (cp)
					predictor = atoi(cp+1);
				compression = COMPRESSION_LZW;
			} else
				usage();
			break;
		case 'p':		
			if (streq(optarg, "separate"))
				config = PLANARCONFIG_SEPARATE;
			else if (streq(optarg, "contig"))
				config = PLANARCONFIG_CONTIG;
			else
				usage();
			break;
		case 'r':		
			rowsperstrip = atoi(optarg);
			break;
		case '?':
			usage();
			
		}
	if (argc - optind != 1 && argc - optind != 5)
		usage();
	xmaxscreen = getgdesc(GD_XPMAX)-1;
	ymaxscreen = getgdesc(GD_YPMAX)-1;
	foreground();
	noport();
	winopen("tiffsv");
	if (argc - optind == 5)
		tiffsv(argv[optind],
		    atoi(argv[optind+1]), atoi(argv[optind+2]),
		    atoi(argv[optind+3]), atoi(argv[optind+4]));
	else
		tiffsv(argv[optind], 0, xmaxscreen, 0, ymaxscreen);
	return (0);
}

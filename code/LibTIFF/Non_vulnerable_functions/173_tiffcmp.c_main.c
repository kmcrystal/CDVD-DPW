int
main(int argc, char* argv[])
{
	TIFF *tif1, *tif2;
	int c, dirnum;
	extern int optind;
	extern char* optarg;
	while ((c = getopt(argc, argv, "ltz:")) != -1)
		switch (c) {
		case 'l':
			stopondiff = 0;
			break;
		case 'z':
			stopondiff = atoi(optarg);
			break;
		case 't':
			stoponfirsttag = 0;
			break;
		case '?':
			usage();
			
		}
	if (argc - optind < 2)
		usage();
	tif1 = TIFFOpen(argv[optind], "r");
	if (tif1 == NULL)
		return (-1);
	tif2 = TIFFOpen(argv[optind+1], "r");
	if (tif2 == NULL)
		return (-2);
	dirnum = 0;
	while (tiffcmp(tif1, tif2)) {
		if (!TIFFReadDirectory(tif1)) {
			if (!TIFFReadDirectory(tif2))
				break;
			printf("No more directories for %s\n",
			    TIFFFileName(tif1));
			return (1);
		} else if (!TIFFReadDirectory(tif2)) {
			printf("No more directories for %s\n",
			    TIFFFileName(tif2));
			return (1);
		}
		printf("Directory %d:\n", ++dirnum);
	}
	TIFFClose(tif1);
	TIFFClose(tif2);
	return (0);
}

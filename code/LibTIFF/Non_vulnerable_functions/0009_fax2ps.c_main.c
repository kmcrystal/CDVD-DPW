int
main(int argc, char** argv)
{
    extern int optind;
    extern char* optarg;
    uint16 *pages = NULL, npages = 0, pageNumber;
    int c, dowarnings = 0;		
    TIFF* tif;
    while ((c = getopt(argc, argv, "l:p:x:y:W:H:wS")) != -1)
	switch (c) {
	case 'H':		
	    pageHeight = (float)atof(optarg);
	    break;
	case 'S':		
	    scaleToPage = 1;
	    break;
	case 'W':		
	    pageWidth = (float)atof(optarg);
	    break;
	case 'p':		
	    pageNumber = (uint16)atoi(optarg);
	    if (pages)
		pages = (uint16*) realloc(pages, (npages+1)*sizeof(uint16));
	    else
		pages = (uint16*) malloc(sizeof(uint16));
	    pages[npages++] = pageNumber;
	    break;
	case 'w':
	    dowarnings = 1;
	    break;
	case 'x':
	    defxres = (float)atof(optarg);
	    break;
	case 'y':
	    defyres = (float)atof(optarg);
	    break;
	case 'l':
	    maxline = atoi(optarg);
	    break;
	case '?':
	    usage(-1);
	}
    if (npages > 0)
	qsort(pages, npages, sizeof(uint16), pcompar);
    if (!dowarnings)
	TIFFSetWarningHandler(0);
    if (optind < argc) {
	do {
	    tif = TIFFOpen(argv[optind], "r");
	    if (tif) {
		fax2ps(tif, npages, pages, argv[optind]);
		TIFFClose(tif);
	    } else
		fprintf(stderr, "%s: Can not open, or not a TIFF file.\n",
		    argv[optind]);
	} while (++optind < argc);
    } else {
	int n;
	FILE* fd;
	char buf[16*1024];
	fd = tmpfile();
	if (fd == NULL) {
	    fprintf(stderr, "Could not obtain temporary file.\n");
	    exit(-2);
	}
#if defined(HAVE_SETMODE) && defined(O_BINARY)
	setmode(fileno(stdin), O_BINARY);
#endif
	while ((n = read(fileno(stdin), buf, sizeof (buf))) > 0)
	    write(fileno(fd), buf, n);
	lseek(fileno(fd), 0, SEEK_SET);
#if defined(_WIN32) && defined(USE_WIN32_FILEIO)
	tif = TIFFFdOpen(_get_osfhandle(fileno(fd)), "temp", "r");
#else
	tif = TIFFFdOpen(fileno(fd), "temp", "r");
#endif
	if (tif) {
	    fax2ps(tif, npages, pages, "<stdin>");
	    TIFFClose(tif);
	} else
	    fprintf(stderr, "Can not open, or not a TIFF file.\n");
	fclose(fd);
    }
    printf("%%%%Trailer\n");
    printf("%%%%Pages: %u\n", totalPages);
    printf("%%%%EOF\n");
    return (0);
}

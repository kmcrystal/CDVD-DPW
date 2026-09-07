int
main(int argc, char* argv[])
{
	FILE *in;
	TIFF *out = NULL;
	TIFFErrorHandler whandler = NULL;
	int compression_in = COMPRESSION_CCITTFAX3;
	int compression_out = COMPRESSION_CCITTFAX3;
	int fillorder_in = FILLORDER_LSB2MSB;
	int fillorder_out = FILLORDER_LSB2MSB;
	uint32 group3options_in = 0;	
	uint32 group3options_out = 0;	
	uint32 group4options_in = 0;	
	uint32 group4options_out = 0;	
	uint32 defrowsperstrip = (uint32) 0;
	uint32 rowsperstrip;
	int photometric_in = PHOTOMETRIC_MINISWHITE;
	int photometric_out = PHOTOMETRIC_MINISWHITE;
	int mode = FAXMODE_CLASSF;
	int rows;
	int c;
	int pn, npages;
	float resY = 196.0;
	extern int optind;
	extern char* optarg;
	while ((c = getopt(argc, argv, "R:X:o:1234ABLMPUW5678abcflmprsuvwz?")) != -1)
		switch (c) {
			
		case '3':		
			compression_in = COMPRESSION_CCITTFAX3;
			break;
		case '4':		
			compression_in = COMPRESSION_CCITTFAX4;
			break;
		case 'U':		
			group3options_in |= GROUP3OPT_UNCOMPRESSED;
			group4options_in |= GROUP4OPT_UNCOMPRESSED;
			break;
		case '1':		
			group3options_in &= ~GROUP3OPT_2DENCODING;
			break;
		case '2':		
			group3options_in |= GROUP3OPT_2DENCODING;
			break;
		case 'P':	
			group3options_in &= ~GROUP3OPT_FILLBITS;
			break;
		case 'A':		
			group3options_in |= GROUP3OPT_FILLBITS;
			break;
		case 'W':		
			photometric_in = PHOTOMETRIC_MINISWHITE;
			break;
		case 'B':		
			photometric_in = PHOTOMETRIC_MINISBLACK;
			break;
		case 'L':		
			fillorder_in = FILLORDER_LSB2MSB;
			break;
		case 'M':		
			fillorder_in = FILLORDER_MSB2LSB;
			break;
		case 'R':		
			resY = (float) atof(optarg);
			break;
		case 'X':		
			xsize = (uint32) atoi(optarg);
			break;
			
		case '7':		
			compression_out = COMPRESSION_CCITTFAX3;
			break;
		case '8':		
			compression_out = COMPRESSION_CCITTFAX4;
			break;
		case 'u':	
			group3options_out |= GROUP3OPT_UNCOMPRESSED;
			group4options_out |= GROUP4OPT_UNCOMPRESSED;
			break;
		case '5':	
			group3options_out &= ~GROUP3OPT_2DENCODING;
			break;
		case '6':	
			group3options_out |= GROUP3OPT_2DENCODING;
			break;
		case 'c':		
			mode = FAXMODE_CLASSIC;
			break;
		case 'f':		
			mode = FAXMODE_CLASSF;
			break;
		case 'm':		
			fillorder_out = FILLORDER_MSB2LSB;
			break;
		case 'l':		
			fillorder_out = FILLORDER_LSB2MSB;
			break;
		case 'o':
			out = TIFFOpen(optarg, "w");
			if (out == NULL) {
				fprintf(stderr,
				    "%s: Can not create or open %s\n",
				    argv[0], optarg);
				return EXIT_FAILURE;
			}
			break;
		case 'a':	
			group3options_out |= GROUP3OPT_FILLBITS;
			break;
		case 'p':	
			group3options_out &= ~GROUP3OPT_FILLBITS;
			break;
		case 'r':		
			defrowsperstrip = atol(optarg);
			break;
		case 's':		
			stretch = 1;
			break;
		case 'w':		
			photometric_out = PHOTOMETRIC_MINISWHITE;
			break;
		case 'b':		
			photometric_out = PHOTOMETRIC_MINISBLACK;
			break;
		case 'z':		
			compression_out = COMPRESSION_LZW;
			break;
		case 'v':		
			verbose++;
			break;
		case '?':
			usage();
			
		}
	npages = argc - optind;
	if (npages < 1)
		usage();
	rowbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	refbuf = _TIFFmalloc(TIFFhowmany8(xsize));
	if (rowbuf == NULL || refbuf == NULL) {
		fprintf(stderr, "%s: Not enough memory\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (out == NULL) {
		out = TIFFOpen("fax.tif", "w");
		if (out == NULL) {
			fprintf(stderr, "%s: Can not create fax.tif\n",
			    argv[0]);
			return (EXIT_FAILURE);
		}
	}
	faxTIFF = TIFFClientOpen("(FakeInput)", "w",
	
				 TIFFClientdata(out),
				 TIFFGetReadProc(out), TIFFGetWriteProc(out),
				 TIFFGetSeekProc(out), TIFFGetCloseProc(out),
				 TIFFGetSizeProc(out), TIFFGetMapFileProc(out),
				 TIFFGetUnmapFileProc(out));
	if (faxTIFF == NULL) {
		fprintf(stderr, "%s: Can not create fake input file\n",
		    argv[0]);
		return (EXIT_FAILURE);
	}
	TIFFSetMode(faxTIFF, O_RDONLY);
	TIFFSetField(faxTIFF, TIFFTAG_IMAGEWIDTH,	xsize);
	TIFFSetField(faxTIFF, TIFFTAG_SAMPLESPERPIXEL,	1);
	TIFFSetField(faxTIFF, TIFFTAG_BITSPERSAMPLE,	1);
	TIFFSetField(faxTIFF, TIFFTAG_FILLORDER,	fillorder_in);
	TIFFSetField(faxTIFF, TIFFTAG_PLANARCONFIG,	PLANARCONFIG_CONTIG);
	TIFFSetField(faxTIFF, TIFFTAG_PHOTOMETRIC,	photometric_in);
	TIFFSetField(faxTIFF, TIFFTAG_YRESOLUTION,	resY);
	TIFFSetField(faxTIFF, TIFFTAG_RESOLUTIONUNIT,	RESUNIT_INCH);
	
	TIFFSetField(faxTIFF, TIFFTAG_COMPRESSION, compression_in);
	if (compression_in == COMPRESSION_CCITTFAX3)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP3OPTIONS, group3options_in);
	else if (compression_in == COMPRESSION_CCITTFAX4)
		TIFFSetField(faxTIFF, TIFFTAG_GROUP4OPTIONS, group4options_in);
	for (pn = 0; optind < argc; pn++, optind++) {
		in = fopen(argv[optind], "rb");
		if (in == NULL) {
			fprintf(stderr,
			    "%s: %s: Can not open\n", argv[0], argv[optind]);
			continue;
		}
#if defined(_WIN32) && defined(USE_WIN32_FILEIO)
                TIFFSetClientdata(faxTIFF, (thandle_t)_get_osfhandle(fileno(in)));
#else
                TIFFSetClientdata(faxTIFF, (thandle_t)fileno(in));
#endif
		TIFFSetFileName(faxTIFF, (const char*)argv[optind]);
		TIFFSetField(out, TIFFTAG_IMAGEWIDTH, xsize);
		TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 1);
		TIFFSetField(out, TIFFTAG_COMPRESSION, compression_out);
		TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric_out);
		TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
		TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
		switch (compression_out) {
			
			case COMPRESSION_CCITTFAX3:
			TIFFSetField(out, TIFFTAG_GROUP3OPTIONS,
				     group3options_out);
			TIFFSetField(out, TIFFTAG_FAXMODE, mode);
			rowsperstrip =
				(defrowsperstrip)?defrowsperstrip:(uint32)-1L;
			break;
			
			case COMPRESSION_CCITTFAX4:
			TIFFSetField(out, TIFFTAG_GROUP4OPTIONS,
				     group4options_out);
			TIFFSetField(out, TIFFTAG_FAXMODE, mode);
			rowsperstrip =
				(defrowsperstrip)?defrowsperstrip:(uint32)-1L;
			break;
			default:
			rowsperstrip = (defrowsperstrip) ?
				defrowsperstrip : TIFFDefaultStripSize(out, 0);
		}
		TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
		TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(out, TIFFTAG_FILLORDER, fillorder_out);
		TIFFSetField(out, TIFFTAG_SOFTWARE, "fax2tiff");
		TIFFSetField(out, TIFFTAG_XRESOLUTION, 204.0);
		if (!stretch) {
			TIFFGetField(faxTIFF, TIFFTAG_YRESOLUTION, &resY);
			TIFFSetField(out, TIFFTAG_YRESOLUTION, resY);
		} else
			TIFFSetField(out, TIFFTAG_YRESOLUTION, 196.);
		TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
		TIFFSetField(out, TIFFTAG_PAGENUMBER, pn, npages);
		if (!verbose)
		    whandler = TIFFSetWarningHandler(NULL);
		rows = copyFaxFile(faxTIFF, out);
		fclose(in);
		if (!verbose)
		    (void) TIFFSetWarningHandler(whandler);
		TIFFSetField(out, TIFFTAG_IMAGELENGTH, rows);
		if (verbose) {
			fprintf(stderr, "%s:\n", argv[optind]);
			fprintf(stderr, "%d rows in input\n", rows);
			fprintf(stderr, "%ld total bad rows\n",
			    (long) badfaxlines);
			fprintf(stderr, "%d max consecutive bad rows\n", badfaxrun);
		}
		if (compression_out == COMPRESSION_CCITTFAX3 &&
		    mode == FAXMODE_CLASSF) {
			TIFFSetField(out, TIFFTAG_BADFAXLINES, badfaxlines);
			TIFFSetField(out, TIFFTAG_CLEANFAXDATA, badfaxlines ?
			    CLEANFAXDATA_REGENERATED : CLEANFAXDATA_CLEAN);
			TIFFSetField(out, TIFFTAG_CONSECUTIVEBADFAXLINES, badfaxrun);
		}
		TIFFWriteDirectory(out);
	}
	TIFFClose(out);
	_TIFFfree(rowbuf);
	_TIFFfree(refbuf);
	return (EXIT_SUCCESS);
}

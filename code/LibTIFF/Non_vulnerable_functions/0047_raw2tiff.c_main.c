int
main(int argc, char* argv[])
{
	uint32	width = 0, length = 0, linebytes, bufsize;
	uint32	nbands = 1;		    
	off_t	hdr_size = 0;		    
	TIFFDataType dtype = TIFF_BYTE;
	int16	depth = 1;		    
	int	swab = 0;		    
	InterleavingType interleaving = 0;  
	uint32  rowsperstrip = (uint32) -1;
	uint16	photometric = PHOTOMETRIC_MINISBLACK;
	uint16	config = PLANARCONFIG_CONTIG;
	uint16	fillorder = FILLORDER_LSB2MSB;
	int	fd;
	char	*outfilename = NULL;
	TIFF	*out;
	uint32 row, col, band;
	int	c;
	unsigned char *buf = NULL, *buf1 = NULL;
	extern int optind;
	extern char* optarg;
	while ((c = getopt(argc, argv, "c:r:H:w:l:b:d:LMp:si:o:h")) != -1) {
		switch (c) {
		case 'c':		
			if (!processCompressOptions(optarg))
				usage();
			break;
		case 'r':		
			rowsperstrip = atoi(optarg);
			break;
		case 'H':		
			hdr_size = atoi(optarg);
			break;
		case 'w':		
			width = atoi(optarg);
			break;
		case 'l':		
			length = atoi(optarg);
			break;
		case 'b':		
			nbands = atoi(optarg);
			break;
		case 'd':		
			if (strncmp(optarg, "byte", 4) == 0)
				dtype = TIFF_BYTE;
			else if (strncmp(optarg, "short", 5) == 0)
				dtype = TIFF_SHORT;
			else if  (strncmp(optarg, "long", 4) == 0)
				dtype = TIFF_LONG;
			else if  (strncmp(optarg, "sbyte", 5) == 0)
				dtype = TIFF_SBYTE;
			else if  (strncmp(optarg, "sshort", 6) == 0)
				dtype = TIFF_SSHORT;
			else if  (strncmp(optarg, "slong", 5) == 0)
				dtype = TIFF_SLONG;
			else if  (strncmp(optarg, "float", 5) == 0)
				dtype = TIFF_FLOAT;
			else if  (strncmp(optarg, "double", 6) == 0)
				dtype = TIFF_DOUBLE;
			else
				dtype = TIFF_BYTE;
			depth = TIFFDataWidth(dtype);
			break;
		case 'L':		
			fillorder = FILLORDER_LSB2MSB;
			break;
		case 'M':		
			fillorder = FILLORDER_MSB2LSB;
			break;
		case 'p':		
			if (strncmp(optarg, "miniswhite", 10) == 0)
				photometric = PHOTOMETRIC_MINISWHITE;
			else if (strncmp(optarg, "minisblack", 10) == 0)
				photometric = PHOTOMETRIC_MINISBLACK;
			else if (strncmp(optarg, "rgb", 3) == 0)
				photometric = PHOTOMETRIC_RGB;
			else if (strncmp(optarg, "cmyk", 4) == 0)
				photometric = PHOTOMETRIC_SEPARATED;
			else if (strncmp(optarg, "ycbcr", 5) == 0)
				photometric = PHOTOMETRIC_YCBCR;
			else if (strncmp(optarg, "cielab", 6) == 0)
				photometric = PHOTOMETRIC_CIELAB;
			else if (strncmp(optarg, "icclab", 6) == 0)
				photometric = PHOTOMETRIC_ICCLAB;
			else if (strncmp(optarg, "itulab", 6) == 0)
				photometric = PHOTOMETRIC_ITULAB;
			else
				photometric = PHOTOMETRIC_MINISBLACK;
			break;
		case 's':		
			swab = 1;
			break;
		case 'i':		
			if (strncmp(optarg, "pixel", 4) == 0)
				interleaving = PIXEL;
			else if  (strncmp(optarg, "band", 6) == 0)
				interleaving = BAND;
			else
				interleaving = 0;
			break;
		case 'o':
			outfilename = optarg;
			break;
		case 'h':
			usage();
		default:
			break;
		}
        }
        if (argc - optind < 2)
		usage();
        fd = open(argv[optind], O_RDONLY|O_BINARY, 0);
	if (fd < 0) {
		fprintf(stderr, "%s: %s: Cannot open input file.\n",
			argv[0], argv[optind]);
		return (-1);
	}
	if (guessSize(fd, dtype, hdr_size, nbands, swab, &width, &length) < 0)
		return 1;
	if (outfilename == NULL)
		outfilename = argv[optind+1];
	out = TIFFOpen(outfilename, "w");
	if (out == NULL) {
		fprintf(stderr, "%s: %s: Cannot open file for output.\n",
			argv[0], outfilename);
		return (-1);
	}
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, length);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, nbands);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, depth * 8);
	TIFFSetField(out, TIFFTAG_FILLORDER, fillorder);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, config);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric);
	switch (dtype) {
	case TIFF_BYTE:
	case TIFF_SHORT:
	case TIFF_LONG:
		TIFFSetField(out, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
		break;
	case TIFF_SBYTE:
	case TIFF_SSHORT:
	case TIFF_SLONG:
		TIFFSetField(out, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
		break;
	case TIFF_FLOAT:
	case TIFF_DOUBLE:
		TIFFSetField(out, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
		break;
	default:
		TIFFSetField(out, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_VOID);
		break;
	}
	if (compression == (uint16) -1)
		compression = COMPRESSION_PACKBITS;
	TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
	switch (compression) {
	case COMPRESSION_JPEG:
		if (photometric == PHOTOMETRIC_RGB
		    && jpegcolormode == JPEGCOLORMODE_RGB)
			photometric = PHOTOMETRIC_YCBCR;
		TIFFSetField(out, TIFFTAG_JPEGQUALITY, quality);
		TIFFSetField(out, TIFFTAG_JPEGCOLORMODE, jpegcolormode);
		break;
	case COMPRESSION_LZW:
	case COMPRESSION_DEFLATE:
		if (predictor != 0)
			TIFFSetField(out, TIFFTAG_PREDICTOR, predictor);
		break;
	}
	switch(interleaving) {
	case BAND:				
		linebytes = width * depth;
		buf = (unsigned char *)_TIFFmalloc(linebytes);
		break;
	case PIXEL:				
	default:
		linebytes = width * nbands * depth;
		break;
	}
	bufsize = width * nbands * depth;
	buf1 = (unsigned char *)_TIFFmalloc(bufsize);
	rowsperstrip = TIFFDefaultStripSize(out, rowsperstrip);
	if (rowsperstrip > length) {
		rowsperstrip = length;
	}
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, rowsperstrip );
	lseek(fd, hdr_size, SEEK_SET);		
	for (row = 0; row < length; row++) {
		switch(interleaving) {
		case BAND:			
			for (band = 0; band < nbands; band++) {
				lseek(fd,
				      hdr_size + (length*band+row)*linebytes,
				      SEEK_SET);
				if (read(fd, buf, linebytes) < 0) {
					fprintf(stderr,
					"%s: %s: scanline %lu: Read error.\n",
					argv[0], argv[optind],
					(unsigned long) row);
				break;
				}
				if (swab)	
					swapBytesInScanline(buf, width, dtype);
				for (col = 0; col < width; col++)
					memcpy(buf1 + (col*nbands+band)*depth,
					       buf + col * depth, depth);
			}
			break;
		case PIXEL:			
		default:
			if (read(fd, buf1, bufsize) < 0) {
				fprintf(stderr,
					"%s: %s: scanline %lu: Read error.\n",
					argv[0], argv[optind],
					(unsigned long) row);
				break;
			}
			if (swab)		
				swapBytesInScanline(buf1, width, dtype);
			break;
		}
		if (TIFFWriteScanline(out, buf1, row, 0) < 0) {
			fprintf(stderr,	"%s: %s: scanline %lu: Write error.\n",
				argv[0], outfilename, (unsigned long) row);
			break;
		}
	}
	if (buf)
		_TIFFfree(buf);
	if (buf1)
		_TIFFfree(buf1);
	TIFFClose(out);
	return (0);
}

int
main(int argc, char* argv[])
{
	unsigned char* buf;
	long row;
	tsize_t linebytes, scanline;
	TIFF *out;
	FILE *in;
	struct rasterfile h;
	uint16 photometric;
	uint16 config = PLANARCONFIG_CONTIG;
	uint32 rowsperstrip = (uint32) -1;
	int c;
#if !HAVE_DECL_OPTARG
	extern int optind;
	extern char* optarg;
#endif
	while ((c = getopt(argc, argv, "c:r:h")) != -1)
		switch (c) {
		case 'c':		
			if (!processCompressOptions(optarg))
				usage();
			break;
		case 'r':		
			rowsperstrip = atoi(optarg);
			break;
		case 'h':
			usage();
			
		}
	if (argc - optind != 2)
		usage();
	in = fopen(argv[optind], "rb");
	if (in == NULL) {
		fprintf(stderr, "%s: Can not open.\n", argv[optind]);
		return (-1);
	}
	if (fread(&h, sizeof (h), 1, in) != 1) {
		fprintf(stderr, "%s: Can not read header.\n", argv[optind]);
		fclose(in);
		return (-2);
	}
	if (strcmp(h.ras_magic, RAS_MAGIC) == 0) {
#ifndef WORDS_BIGENDIAN
			TIFFSwabLong((uint32 *)&h.ras_width);
			TIFFSwabLong((uint32 *)&h.ras_height);
			TIFFSwabLong((uint32 *)&h.ras_depth);
			TIFFSwabLong((uint32 *)&h.ras_length);
			TIFFSwabLong((uint32 *)&h.ras_type);
			TIFFSwabLong((uint32 *)&h.ras_maptype);
			TIFFSwabLong((uint32 *)&h.ras_maplength);
#endif
	} else if (strcmp(h.ras_magic, RAS_MAGIC_INV) == 0) {
#ifdef WORDS_BIGENDIAN
			TIFFSwabLong((uint32 *)&h.ras_width);
			TIFFSwabLong((uint32 *)&h.ras_height);
			TIFFSwabLong((uint32 *)&h.ras_depth);
			TIFFSwabLong((uint32 *)&h.ras_length);
			TIFFSwabLong((uint32 *)&h.ras_type);
			TIFFSwabLong((uint32 *)&h.ras_maptype);
			TIFFSwabLong((uint32 *)&h.ras_maplength);
#endif
	} else {
		fprintf(stderr, "%s: Not a rasterfile.\n", argv[optind]);
		fclose(in);
		return (-3);
	}
        if ((h.ras_width <= 0) || (h.ras_width >= INT_MAX) ||
            (h.ras_height <= 0) || (h.ras_height >= INT_MAX) ||
            (h.ras_depth <= 0) || (h.ras_depth >= INT_MAX) ||
            (h.ras_length <= 0) || (h.ras_length >= INT_MAX) ||
            (h.ras_type <= 0) ||
            (h.ras_maptype <= 0) ||
            (h.ras_maplength <= 0) || (h.ras_maplength >= INT_MAX)) {
                fprintf(stderr, "%s: Improper image header.\n", argv[optind]);
                fclose(in);
		return (-2);
        }
        if ((h.ras_depth != 1) &&
            (h.ras_depth != 8) &&
            (h.ras_depth != 24)) {
                fprintf(stderr, "%s: Improper image depth (%d).\n",
                        argv[optind], h.ras_depth);
                fclose(in);
		return (-2);
        }
	out = TIFFOpen(argv[optind+1], "w");
	if (out == NULL)
	{
		fclose(in);
		return (-4);
	}
	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, (uint32) h.ras_width);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, (uint32) h.ras_height);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, h.ras_depth > 8 ? 3 : 1);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, h.ras_depth > 1 ? 8 : 1);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, config);
	if (h.ras_maptype != RMT_NONE) {
		uint16* red;
		register uint16* map;
		register int i, j;
		int mapsize;
		buf = (unsigned char *)_TIFFmalloc(h.ras_maplength);
		if (buf == NULL) {
			fprintf(stderr, "No space to read in colormap.\n");
			return (-5);
		}
		if (fread(buf, h.ras_maplength, 1, in) != 1) {
			fprintf(stderr, "%s: Read error on colormap.\n",
			    argv[optind]);
			return (-6);
		}
		mapsize = 1<<h.ras_depth; 
		if (h.ras_maplength > mapsize*3) {
			fprintf(stderr,
			    "%s: Huh, %d colormap entries, should be %d?\n",
			    argv[optind], h.ras_maplength, mapsize*3);
			return (-7);
		}
		red = (uint16*)_TIFFmalloc(mapsize * 3 * sizeof (uint16));
		if (red == NULL) {
			fprintf(stderr, "No space for colormap.\n");
			return (-8);
		}
		map = red;
		for (j = 0; j < 3; j++) {
#define	SCALE(x)	(((x)*((1L<<16)-1))/255)
			for (i = h.ras_maplength/3; i-- > 0;)
				*map++ = SCALE(*buf++);
			if ((i = h.ras_maplength/3) < mapsize) {
				i = mapsize - i;
				_TIFFmemset(map, 0, i*sizeof (uint16));
				map += i;
			}
		}
		TIFFSetField(out, TIFFTAG_COLORMAP,
		     red, red + mapsize, red + 2*mapsize);
		photometric = PHOTOMETRIC_PALETTE;
		if (compression == (uint16) -1)
			compression = COMPRESSION_PACKBITS;
		TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
	} else {
		
		photometric = h.ras_depth == 24 ?
		    PHOTOMETRIC_RGB : PHOTOMETRIC_MINISBLACK;
		if (compression == (uint16) -1)
			compression = COMPRESSION_LZW;
		TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
	}
	switch (compression) {
	case COMPRESSION_JPEG:
		if (photometric == PHOTOMETRIC_RGB && jpegcolormode == JPEGCOLORMODE_RGB)
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
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photometric);
	linebytes = ((h.ras_depth*h.ras_width+15) >> 3) &~ 1;
	scanline = TIFFScanlineSize(out);
	if (scanline > linebytes) {
		buf = (unsigned char *)_TIFFmalloc(scanline);
		_TIFFmemset(buf+linebytes, 0, scanline-linebytes);
	} else
		buf = (unsigned char *)_TIFFmalloc(linebytes);
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP,
	    TIFFDefaultStripSize(out, rowsperstrip));
	for (row = 0; row < h.ras_height; row++) {
		if (fread(buf, linebytes, 1, in) != 1) {
			fprintf(stderr, "%s: scanline %ld: Read error.\n",
			    argv[optind], row);
			break;
		}
		if (h.ras_type == RT_STANDARD && h.ras_depth == 24) {
			tsize_t cc = h.ras_width;
			unsigned char* cp = buf;
#define	SWAP(a,b)	{ unsigned char t = (a); (a) = (b); (b) = t; }
			do {
				SWAP(cp[0], cp[2]);
				cp += 3;
			} while (--cc);
		}
		if (TIFFWriteScanline(out, buf, row, 0) < 0)
			break;
	}
	(void) TIFFClose(out);
	fclose(in);
	return (0);
}

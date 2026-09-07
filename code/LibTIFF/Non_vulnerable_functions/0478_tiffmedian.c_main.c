int
main(int argc, char* argv[])
{
	int i, dither = 0;
	uint16 shortv, config, photometric;
	Colorbox *box_list, *ptr;
	float floatv;
	uint32 longv;
	int c;
#if !HAVE_DECL_OPTARG
	extern int optind;
	extern char* optarg;
#endif
	num_colors = MAX_CMAP_SIZE;
	while ((c = getopt(argc, argv, "c:C:r:f")) != -1)
		switch (c) {
		case 'c':		
			if (!processCompressOptions(optarg))
				usage();
			break;
		case 'C':		
			num_colors = atoi(optarg);
			if (num_colors > MAX_CMAP_SIZE) {
				fprintf(stderr,
				   "-c: colormap too big, max %d\n",
				   MAX_CMAP_SIZE);
				usage();
			}
			break;
		case 'f':		
			dither = 1;
			break;
		case 'r':		
			rowsperstrip = atoi(optarg);
			break;
		case '?':
			usage();
			
		}
	if (argc - optind != 2)
		usage();
	in = TIFFOpen(argv[optind], "r");
	if (in == NULL)
		return (-1);
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &imagewidth);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &imagelength);
	TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
	TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
	if (bitspersample != 8 && bitspersample != 16) {
		fprintf(stderr, "%s: Image must have at least 8-bits/sample\n",
		    argv[optind]);
		return (-3);
	}
	if (!TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &photometric) ||
	    photometric != PHOTOMETRIC_RGB || samplesperpixel < 3) {
		fprintf(stderr, "%s: Image must have RGB data\n", argv[optind]);
		return (-4);
	}
	TIFFGetField(in, TIFFTAG_PLANARCONFIG, &config);
	if (config != PLANARCONFIG_CONTIG) {
		fprintf(stderr, "%s: Can only handle contiguous data packing\n",
		    argv[optind]);
		return (-5);
	}
	


	usedboxes = NULL;
	box_list = freeboxes = (Colorbox *)_TIFFmalloc(num_colors*sizeof (Colorbox));
	freeboxes[0].next = &freeboxes[1];
	freeboxes[0].prev = NULL;
	for (i = 1; i < num_colors-1; ++i) {
		freeboxes[i].next = &freeboxes[i+1];
		freeboxes[i].prev = &freeboxes[i-1];
	}
	freeboxes[num_colors-1].next = NULL;
	freeboxes[num_colors-1].prev = &freeboxes[num_colors-2];
	


	ptr = freeboxes;
	freeboxes = ptr->next;
	if (freeboxes)
		freeboxes->prev = NULL;
	ptr->next = usedboxes;
	usedboxes = ptr;
	if (ptr->next)
		ptr->next->prev = ptr;
	get_histogram(in, ptr);
	



	while (freeboxes != NULL) {
		ptr = largest_box();
		if (ptr != NULL)
			splitbox(ptr);
		else
			freeboxes = NULL;
	}
	


	for (i = 0, ptr = usedboxes; ptr != NULL; ++i, ptr = ptr->next) {
		rm[i] = ((ptr->rmin + ptr->rmax) << COLOR_SHIFT) / 2;
		gm[i] = ((ptr->gmin + ptr->gmax) << COLOR_SHIFT) / 2;
		bm[i] = ((ptr->bmin + ptr->bmax) << COLOR_SHIFT) / 2;
	}
	
	_TIFFfree(box_list);
	freeboxes = usedboxes = NULL;
	


	
	ColorCells = (C_cell **)_TIFFmalloc(C_LEN*C_LEN*C_LEN*sizeof (C_cell*));
	_TIFFmemset(ColorCells, 0, C_LEN*C_LEN*C_LEN*sizeof (C_cell*));
	

	map_colortable();
	


	out = TIFFOpen(argv[optind+1], "w");
	if (out == NULL)
		return (-2);
	CopyField(TIFFTAG_SUBFILETYPE, longv);
	CopyField(TIFFTAG_IMAGEWIDTH, longv);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, (short)COLOR_DEPTH);
	if (compression != (uint16)-1) {
		TIFFSetField(out, TIFFTAG_COMPRESSION, compression);
		switch (compression) {
		case COMPRESSION_LZW:
		case COMPRESSION_DEFLATE:
			if (predictor != 0)
				TIFFSetField(out, TIFFTAG_PREDICTOR, predictor);
			break;
		}
	} else
		CopyField(TIFFTAG_COMPRESSION, compression);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, (short)PHOTOMETRIC_PALETTE);
	CopyField(TIFFTAG_ORIENTATION, shortv);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, (short)1);
	CopyField(TIFFTAG_PLANARCONFIG, shortv);
	TIFFSetField(out, TIFFTAG_ROWSPERSTRIP,
	    TIFFDefaultStripSize(out, rowsperstrip));
	CopyField(TIFFTAG_MINSAMPLEVALUE, shortv);
	CopyField(TIFFTAG_MAXSAMPLEVALUE, shortv);
	CopyField(TIFFTAG_RESOLUTIONUNIT, shortv);
	CopyField(TIFFTAG_XRESOLUTION, floatv);
	CopyField(TIFFTAG_YRESOLUTION, floatv);
	CopyField(TIFFTAG_XPOSITION, floatv);
	CopyField(TIFFTAG_YPOSITION, floatv);
	if (dither)
		quant_fsdither(in, out);
	else
		quant(in, out);
	


#define	SCALE(x)	(((x)*((1L<<16)-1))/255)
	for (i = 0; i < MAX_CMAP_SIZE; ++i) {
		rm[i] = SCALE(rm[i]);
		gm[i] = SCALE(gm[i]);
		bm[i] = SCALE(bm[i]);
	}
	TIFFSetField(out, TIFFTAG_COLORMAP, rm, gm, bm);
	(void) TIFFClose(out);
	return (0);
}

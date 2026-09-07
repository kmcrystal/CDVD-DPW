  */
int main(int argc, char** argv){
#if !HAVE_DECL_OPTARG
	extern char *optarg;
	extern int optind;
#endif
	const char *outfilename = NULL;
	T2P *t2p = NULL;
	TIFF *input = NULL, *output = NULL;
	int c, ret = EXIT_SUCCESS;
	t2p = t2p_init();
	if (t2p == NULL){
		TIFFError(TIFF2PDF_MODULE, "Can't initialize context");
		goto fail;
	}
	while (argv &&
	       (c = getopt(argc, argv,
			   "o:q:u:x:y:w:l:r:p:e:c:a:t:s:k:jzndifbhF")) != -1){
		switch (c) {
			case 'o':
				outfilename = optarg;
				break;
#ifdef JPEG_SUPPORT
			case 'j':  
				t2p->pdf_defaultcompression=T2P_COMPRESS_JPEG;
				break;
#endif
#ifndef JPEG_SUPPORT
			case 'j':  
				TIFFWarning(
					TIFF2PDF_MODULE, 
					"JPEG support in libtiff required for JPEG compression, ignoring option");
				break;
#endif
#ifdef ZIP_SUPPORT
			case 'z':  
				t2p->pdf_defaultcompression=T2P_COMPRESS_ZIP;
				break;
#endif
#ifndef ZIP_SUPPORT
			case 'z':  
				TIFFWarning(
					TIFF2PDF_MODULE, 
					"Zip support in libtiff required for Zip compression, ignoring option");
				break;
#endif
			case 'q': 
				t2p->pdf_defaultcompressionquality=atoi(optarg);
				break;
			case 'n': 
				t2p->pdf_nopassthrough=1;
				break;
			case 'd': 
				t2p->pdf_defaultcompression=T2P_COMPRESS_NONE;
				break;
			case 'u': 
				if(optarg[0]=='m'){
					t2p->pdf_centimeters=1;
				}
				break;
			case 'x': 
				t2p->pdf_defaultxres = 
					(float)atof(optarg) / (t2p->pdf_centimeters?2.54F:1.0F);
				break;
			case 'y': 
				t2p->pdf_defaultyres = 
					(float)atof(optarg) / (t2p->pdf_centimeters?2.54F:1.0F);
				break;
			case 'w': 
				t2p->pdf_overridepagesize=1;
				t2p->pdf_defaultpagewidth = 
					((float)atof(optarg) * PS_UNIT_SIZE) / (t2p->pdf_centimeters?2.54F:1.0F);
				break;
			case 'l': 
				t2p->pdf_overridepagesize=1;
				t2p->pdf_defaultpagelength = 
					((float)atof(optarg) * PS_UNIT_SIZE) / (t2p->pdf_centimeters?2.54F:1.0F);
				break;
			case 'r': 
				if(optarg[0]=='o'){
					t2p->pdf_overrideres=1;
				}
				break;
			case 'p': 
				if(tiff2pdf_match_paper_size(
					&(t2p->pdf_defaultpagewidth), 
					&(t2p->pdf_defaultpagelength), 
					optarg)){
					t2p->pdf_overridepagesize=1;
				} else {
					TIFFWarning(TIFF2PDF_MODULE, 
					"Unknown paper size %s, ignoring option",
						optarg);
				}
				break;
			case 'i':
				t2p->pdf_colorspace_invert=1;
				break;
			case 'F':
				t2p->pdf_image_fillpage = 1;
				break;
			case 'f': 
				t2p->pdf_fitwindow=1;
				break;
			case 'e':
				if (strlen(optarg) == 0) {
					t2p->pdf_datetime[0] = '\0';
				} else {
					t2p->pdf_datetime[0] = 'D';
					t2p->pdf_datetime[1] = ':';
					strncpy(t2p->pdf_datetime + 2, optarg,
						sizeof(t2p->pdf_datetime) - 3);
					t2p->pdf_datetime[sizeof(t2p->pdf_datetime) - 1] = '\0';
				}
				break;
			case 'c': 
				strncpy(t2p->pdf_creator, optarg, sizeof(t2p->pdf_creator) - 1);
				t2p->pdf_creator[sizeof(t2p->pdf_creator) - 1] = '\0';
				break;
			case 'a': 
				strncpy(t2p->pdf_author, optarg, sizeof(t2p->pdf_author) - 1);
				t2p->pdf_author[sizeof(t2p->pdf_author) - 1] = '\0';
				break;
			case 't': 
				strncpy(t2p->pdf_title, optarg, sizeof(t2p->pdf_title) - 1);
				t2p->pdf_title[sizeof(t2p->pdf_title) - 1] = '\0';
				break;
			case 's': 
				strncpy(t2p->pdf_subject, optarg, sizeof(t2p->pdf_subject) - 1);
				t2p->pdf_subject[sizeof(t2p->pdf_subject) - 1] = '\0';
				break;
			case 'k': 
				strncpy(t2p->pdf_keywords, optarg, sizeof(t2p->pdf_keywords) - 1);
				t2p->pdf_keywords[sizeof(t2p->pdf_keywords) - 1] = '\0';
				break;
			case 'b':
				t2p->pdf_image_interpolate = 1;
				break;
			case 'h': 
			case '?': 
				tiff2pdf_usage();
				goto success;
				break;
		}
	}
	


	if(argc > optind) {
		input = TIFFOpen(argv[optind++], "r");
		if (input==NULL) {
			TIFFError(TIFF2PDF_MODULE, 
				  "Can't open input file %s for reading", 
				  argv[optind-1]);
			goto fail;
		}
	} else {
		TIFFError(TIFF2PDF_MODULE, "No input file specified"); 
		tiff2pdf_usage();
		goto fail;
	}
	if(argc > optind) {
		TIFFError(TIFF2PDF_MODULE, 
			  "No support for multiple input files"); 
		tiff2pdf_usage();
		goto fail;
	}
	


	t2p->outputdisable = 1;
	if (outfilename) {
		t2p->outputfile = fopen(outfilename, "wb");
		if (t2p->outputfile == NULL) {
			TIFFError(TIFF2PDF_MODULE,
				  "Can't open output file %s for writing",
				  outfilename);
			goto fail;
		}
	} else {
		outfilename = "-";
		t2p->outputfile = stdout;
	}
	output = TIFFClientOpen(outfilename, "w", (thandle_t) t2p,
				t2p_readproc, t2p_writeproc, t2p_seekproc, 
				t2p_closeproc, t2p_sizeproc, 
				t2p_mapproc, t2p_unmapproc);
	t2p->outputdisable = 0;
	if (output == NULL) {
		TIFFError(TIFF2PDF_MODULE,
			  "Can't initialize output descriptor");
		goto fail;
	}
	


	t2p_validate(t2p);
	t2pSeekFile(output, (toff_t) 0, SEEK_SET);
	


	t2p_write_pdf(t2p, input, output);
	if (t2p->t2p_error != 0) {
		TIFFError(TIFF2PDF_MODULE,
			  "An error occurred creating output PDF file");
		goto fail;
	}
	goto success;
fail:
	ret = EXIT_FAILURE;
success:
	if(input != NULL)
		TIFFClose(input);
	if (output != NULL)
		TIFFClose(output);
	if (t2p != NULL)
		t2p_free(t2p);
	return ret;
}

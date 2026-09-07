main(int argc, char **argv)
{
    char emsg[1024];
    TIFFImageIter img;
    int ok;
    int stop = 1;
    TIFF *tif;
    unsigned long nx, ny;
    unsigned short BitsPerSample, SamplesPerPixel;
    int isColorMapped, isPliFile;
    unsigned char *ColorMap;
    unsigned char *data;
    if (argc < 2) {
	fprintf(stderr,"usage: %s tiff_file\n",argv[0]);
	exit(1);
    }
    tif = (TIFF *)PLIGetImage(argv[1], (void *) &data, &ColorMap, 
			      &nx, &ny, &BitsPerSample, &SamplesPerPixel, 
			      &isColorMapped, &isPliFile);
    if (tif != NULL) {
	if (TIFFImageIterBegin(&img, tif, stop, emsg)) {
	    
	    if (img.isContig) {
		img.callback = TestContigCallback;
	    } else {
		img.callback = TestSepCallback;
	    }
	    ok = TIFFImageIterGet(&img, NULL, img.width, img.height);
	    TIFFImageIterEnd(&img);
	} else {
	    TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), emsg);
	}
    }
}

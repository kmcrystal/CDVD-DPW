void
GetTIFFHeader()
{
    register int i;
    if (!TIFFSetDirectory(tfFile, tfDirectory)) {
        fprintf(stderr, "xtiff: can't seek to directory %d in %s\n",
            tfDirectory, fileName);
        exit(0);
    }
    TIFFGetField(tfFile, TIFFTAG_IMAGEWIDTH, &tfImageWidth);
    TIFFGetField(tfFile, TIFFTAG_IMAGELENGTH, &tfImageHeight);
    


    TIFFGetFieldDefaulted(tfFile, TIFFTAG_BITSPERSAMPLE, &tfBitsPerSample);
    TIFFGetFieldDefaulted(tfFile, TIFFTAG_SAMPLESPERPIXEL, &tfSamplesPerPixel);
    TIFFGetFieldDefaulted(tfFile, TIFFTAG_PLANARCONFIG, &tfPlanarConfiguration);
    TIFFGetFieldDefaulted(tfFile, TIFFTAG_GRAYRESPONSEUNIT, &tfGrayResponseUnit);
    tfUnitMap = tfGrayResponseUnitMap[tfGrayResponseUnit];
    colormapSize = 1 << tfBitsPerSample;
    tfImageDepth = tfBitsPerSample * tfSamplesPerPixel;
    dRed = (double *) malloc(colormapSize * sizeof(double));
    dGreen = (double *) malloc(colormapSize * sizeof(double));
    dBlue = (double *) malloc(colormapSize * sizeof(double));
    MCHECK(dRed); MCHECK(dGreen); MCHECK(dBlue);
    



    if (!TIFFGetField(tfFile, TIFFTAG_PHOTOMETRIC,
            &tfPhotometricInterpretation)) {
        if (tfSamplesPerPixel != 1)
            tfPhotometricInterpretation = PHOTOMETRIC_RGB;
        else if (tfBitsPerSample == 1)
            tfPhotometricInterpretation = PHOTOMETRIC_MINISBLACK;
        else if (TIFFGetField(tfFile, TIFFTAG_COLORMAP,
                &redMap, &greenMap, &blueMap)) {
            tfPhotometricInterpretation = PHOTOMETRIC_PALETTE;
            redMap = greenMap = blueMap = NULL;
        } else
            tfPhotometricInterpretation = PHOTOMETRIC_MINISBLACK;
    }
    


    switch (tfPhotometricInterpretation) {
    case PHOTOMETRIC_RGB:
	redMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
	greenMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
	blueMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
	MCHECK(redMap); MCHECK(greenMap); MCHECK(blueMap);
	for (i = 0; i < colormapSize; i++)
	    dRed[i] = dGreen[i] = dBlue[i]
		= (double) SCALE(i, colormapSize - 1);
        break;
    case PHOTOMETRIC_PALETTE:
        if (!TIFFGetField(tfFile, TIFFTAG_COLORMAP,
                &redMap, &greenMap, &blueMap)) {
            redMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
            greenMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
            blueMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
            MCHECK(redMap); MCHECK(greenMap); MCHECK(blueMap);
            for (i = 0; i < colormapSize; i++)
                dRed[i] = dGreen[i] = dBlue[i]
                    = (double) SCALE(i, colormapSize - 1);
        } else {
            CheckAndCorrectColormap();
            for (i = 0; i < colormapSize; i++) {
                dRed[i] = (double) redMap[i];
                dGreen[i] = (double) greenMap[i];
                dBlue[i] = (double) blueMap[i];
            }
        }
        break;
    case PHOTOMETRIC_MINISWHITE:
        redMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        greenMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        blueMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        MCHECK(redMap); MCHECK(greenMap); MCHECK(blueMap);
	for (i = 0; i < colormapSize; i++)
	    dRed[i] = dGreen[i] = dBlue[i] = (double)
		 SCALE(colormapSize-1-i, colormapSize-1);
        break;
    case PHOTOMETRIC_MINISBLACK:
        redMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        greenMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        blueMap = (uint16 *) malloc(colormapSize * sizeof(uint16));
        MCHECK(redMap); MCHECK(greenMap); MCHECK(blueMap);
	for (i = 0; i < colormapSize; i++)
	    dRed[i] = dGreen[i] = dBlue[i] = (double) SCALE(i, colormapSize-1);
        break;
    default:
        fprintf(stderr,
            "xtiff: can't display photometric interpretation type %d\n",
            tfPhotometricInterpretation);
        exit(0);
    }
}

void
GetVisual()
{
    XColor *colors = NULL;
    unsigned long *pixels = NULL;
    unsigned long i;
    switch (tfImageDepth) {
    



    case 32:
    case 24:
        if (SearchVisualList(24, DirectColor, &xVisual) == False) {
            fprintf(stderr, "xtiff: 24-bit DirectColor visual not available\n");
            exit(0);
        }
        colors = (XColor *) malloc(3 * colormapSize * sizeof(XColor));
        MCHECK(colors);
        for (i = 0; i < colormapSize; i++) {
            colors[i].pixel = (i << 16) + (i << 8) + i;
            colors[i].red = redMap[i];
            colors[i].green = greenMap[i];
            colors[i].blue = blueMap[i];
            colors[i].flags = DoRed | DoGreen | DoBlue;
        }
        xColormap = XCreateColormap(xDisplay, RootWindow(xDisplay, xScreen),
            xVisual, AllocAll);
        XStoreColors(xDisplay, xColormap, colors, colormapSize);
        break;
    case 8:
    case 4:
    case 2:
        



        switch (tfPhotometricInterpretation) {
        case PHOTOMETRIC_MINISWHITE:
        case PHOTOMETRIC_MINISBLACK:
            if (SearchVisualList((int) tfImageDepth, GrayScale, &xVisual) == True)
                break;
        case PHOTOMETRIC_PALETTE:
            if (SearchVisualList((int) tfImageDepth, PseudoColor, &xVisual) == True)
                break;
        default:
            fprintf(stderr, "xtiff: Unsupported TIFF/X configuration\n");
            exit(0);
        }
        colors = (XColor *) malloc(colormapSize * sizeof(XColor));
        MCHECK(colors);
        for (i = 0; i < colormapSize; i++) {
            colors[i].pixel = i;
            colors[i].red = redMap[i];
            colors[i].green = greenMap[i];
            colors[i].blue = blueMap[i];
            colors[i].flags = DoRed | DoGreen | DoBlue;
        }
        








        if (tfImageDepth == 8)
            xColormap = XCreateColormap(xDisplay, RootWindow(xDisplay, xScreen),
                xVisual, AllocAll);
        else {
            xColormap = XCreateColormap(xDisplay, RootWindow(xDisplay, xScreen),
                xVisual, AllocNone);
            pixels = (unsigned long *)
                malloc(colormapSize * sizeof(unsigned long));
            MCHECK(pixels);
            (void) XAllocColorCells(xDisplay, xColormap, True,
                NULL, 0, pixels, colormapSize);
            basePixel = (unsigned char) pixels[0];
            free(pixels);
        }
        XStoreColors(xDisplay, xColormap, colors, colormapSize);
        break;
    case 1:
        xImageDepth = 1;
        xVisual = DefaultVisual(xDisplay, xScreen);
        xColormap = DefaultColormap(xDisplay, xScreen);
        break;
    default:
        fprintf(stderr, "xtiff: unsupported image depth %d\n", tfImageDepth);
        exit(0);
    }
    if (appData.verbose == True)
	fprintf(stderr, "%s: Using %d-bit %s visual.\n",
	    fileName, xImageDepth, classNames[xVisual->class]);
    if (colors != NULL)
        free(colors);
    if (grayMap != NULL)
        free(grayMap);
    if (redMap != NULL)
        free(redMap);
    if (greenMap != NULL)
        free(greenMap);
    if (blueMap != NULL)
        free(blueMap);
    colors = NULL; grayMap = redMap = greenMap = blueMap = NULL;
}

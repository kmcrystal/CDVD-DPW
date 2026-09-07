int main(int argc, char **argv)
{
    int             i;
    unsigned char * scan_line;
    TIFF *          tif;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s tiff-image\n", argv[0]);
        return 0;
    }
    if ((tif = TIFFOpen(argv[1], "w")) == NULL) {
        fprintf(stderr, "can't open %s as a TIFF file\n", argv[1]);
        return 0;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, WIDTH);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, HEIGHT);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_NONE);
    scan_line = (unsigned char *) malloc(WIDTH / 8);
    for (i = 0; i < (WIDTH / 8) / 2; i++)
        scan_line[i] = 0;
    for (i = (WIDTH / 8) / 2; i < (WIDTH / 8); i++)
        scan_line[i] = 255;
    for (i = 0; i < HEIGHT / 2; i++)
        TIFFWriteScanline(tif, scan_line, i, 0);
    for (i = 0; i < (WIDTH / 8) / 2; i++)
        scan_line[i] = 255;
    for (i = (WIDTH / 8) / 2; i < (WIDTH / 8); i++)
        scan_line[i] = 0;
    for (i = HEIGHT / 2; i < HEIGHT; i++)
        TIFFWriteScanline(tif, scan_line, i, 0);
    free(scan_line);
    TIFFClose(tif);
    return 0;
}

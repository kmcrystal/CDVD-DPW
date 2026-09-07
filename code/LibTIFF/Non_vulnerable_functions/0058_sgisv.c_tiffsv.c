static void
tiffsv(char* name, int x1, int x2, int y1, int y2)
{
	TIFF *tif;
	int xsize, ysize;
	int xorg, yorg;
	uint32 *scrbuf;
	xorg = MIN(x1,x2);
	yorg = MIN(y1,y2);
	if (xorg<0)
		xorg = 0;
	if (yorg<0)
		yorg = 0;
	xsize = ABS(x2-x1);
	ysize = ABS(y2-y1);
	if (xorg+xsize > xmaxscreen)
		xsize = xmaxscreen-xorg;
	if (yorg+ysize > ymaxscreen)
		ysize = ymaxscreen-yorg;
	tif = TIFFOpen(name, "w");
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32) (xsize+1));
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32) (ysize+1));
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL,
	    photometric == PHOTOMETRIC_RGB ? 3 : 1);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, config);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
	switch (compression) {
	case COMPRESSION_JPEG:
		if (photometric == PHOTOMETRIC_RGB && jpegcolormode == JPEGCOLORMODE_RGB)
			photometric = PHOTOMETRIC_YCBCR;
		TIFFSetField(tif, TIFFTAG_JPEGQUALITY, quality);
		TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, jpegcolormode);
		break;
	case COMPRESSION_LZW:
		if (predictor != 0)
			TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
		break;
	}
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
	rowsperstrip = TIFFDefaultStripSize(tif, rowsperstrip);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	scrbuf = (uint32 *)_TIFFmalloc((xsize+1)*(ysize+1)*sizeof (uint32));
	readdisplay(xorg, yorg, xorg+xsize, yorg+ysize, scrbuf, RD_FREEZE);
	if (photometric == PHOTOMETRIC_RGB) {
		if (config == PLANARCONFIG_SEPARATE)
			svRGBSeparate(tif, scrbuf, xsize, ysize);
		else
			svRGBContig(tif, scrbuf, xsize, ysize);
	} else
		svGrey(tif, scrbuf, xsize, ysize);
	(void) TIFFClose(tif);
	_TIFFfree((char *)scrbuf);
}

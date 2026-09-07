static void
svGrey(TIFF* tif, uint32* ss, int xsize, int ysize)
{
	register int x, y;
	unsigned char *buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));
	for (y = 0; y <= ysize; y++) {
		for (x = 0; x <= xsize; x++) {
			unsigned char *cp = (unsigned char *)&ss[x];
			buf[x] = (RED*cp[3] + GREEN*cp[2] + BLUE*cp[1]) >> 8;
		}
		if (TIFFWriteScanline(tif, buf, (uint32) y, 0) < 0)
			break;
		ss += xsize+1;
	}
	_TIFFfree(buf);
}

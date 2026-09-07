static void
svRGBContig(TIFF* tif, uint32* ss, int xsize, int ysize)
{
	register int x, y;
	tsize_t stripsize = TIFFStripSize(tif);
	unsigned char *strip = (unsigned char *)_TIFFmalloc(stripsize);
	for (y = 0; y <= ysize; y += rowsperstrip) {
		register unsigned char *pp = strip;
		register uint32 n;
		n = rowsperstrip;
		if (n > ysize-y+1)
			n = ysize-y+1;
		do {
			for (x = 0; x <= xsize; x++) {
				uint32 v = ss[x];
				pp[0] = v;
				pp[1] = v >> 8;
				pp[2] = v >> 16;
				pp += 3;
			}
			ss += xsize+1;
		} while (--n);
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,0),
		    strip, stripsize) < 0)
			break;
	}
	_TIFFfree(strip);
}

static void
svRGBSeparate(TIFF* tif, uint32* ss, int xsize, int ysize)
{
	tsize_t stripsize = TIFFStripSize(tif);
	unsigned char *rbuf = (unsigned char *)_TIFFmalloc(3*stripsize);
	unsigned char *gbuf = rbuf + stripsize;
	unsigned char *bbuf = gbuf + stripsize;
	register int y;
	for (y = 0; y <= ysize; y += rowsperstrip) {
		unsigned char *rp, *gp, *bp;
		register int x;
		register uint32 n;
		n = rowsperstrip;
		if (n > ysize-y+1)
			n = ysize-y+1;
		rp = rbuf; gp = gbuf; bp = bbuf;
		do {
			for (x = 0; x <= xsize; x++) {
				uint32 v = ss[x];
				rp[x] = v;
				gp[x] = v >> 8;
				bp[x] = v >> 16;
			}
			rp += xsize+1, gp += xsize+1, bp += xsize+1;
			ss += xsize+1;
		} while (--n);
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,0),
		    rbuf, stripsize) < 0)
			break;
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,1),
		    gbuf, stripsize) < 0)
			break;
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,2),
		    bbuf, stripsize) < 0)
			break;
	}
	_TIFFfree(rbuf);
}

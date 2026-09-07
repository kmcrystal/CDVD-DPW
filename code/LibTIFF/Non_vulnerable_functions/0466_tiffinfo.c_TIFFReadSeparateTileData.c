void
TIFFReadSeparateTileData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t rowsize = TIFFTileRowSize(tif);
	buf = (unsigned char *)_TIFFmalloc(TIFFTileSize(tif));
	if (buf) {
		uint32 tw, th, w, h;
		uint32 row, col;
		tsample_t s, samplesperpixel;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
		TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		for (row = 0; row < h; row += th) {
			for (col = 0; col < w; col += tw) {
				for (s = 0; s < samplesperpixel; s++) {
					if (TIFFReadTile(tif, buf, col, row, 0, s) < 0) {
						if (stoponerr)
							break;
					} else if (showdata)
						ShowTile(row, col, s, buf, th, rowsize);
				}
			}
		}
		_TIFFfree(buf);
	}
}

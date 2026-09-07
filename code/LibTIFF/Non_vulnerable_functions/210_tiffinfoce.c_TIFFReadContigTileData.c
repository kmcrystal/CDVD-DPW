void
TIFFReadContigTileData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t rowsize = TIFFTileRowSize(tif);
	buf = (unsigned char *)_TIFFmalloc(TIFFTileSize(tif));
	if (buf) {
		uint32 tw, th, w, h;
		uint32 row, col;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
		TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
		for (row = 0; row < h; row += th) {
			for (col = 0; col < w; col += tw) {
				if (TIFFReadTile(tif, buf, col, row, 0, 0) < 0) {
					if (stoponerr)
						break;
				} else if (showdata)
					ShowTile(row, col, (tsample_t) -1, buf, th, rowsize);
			}
		}
		_TIFFfree(buf);
	}
}

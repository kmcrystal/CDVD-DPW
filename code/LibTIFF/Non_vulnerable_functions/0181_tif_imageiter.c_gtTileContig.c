static int
gtTileContig(TIFFImageIter* img, void *udata, uint32 w, uint32 h)
{
    TIFF* tif = img->tif;
    ImageIterTileContigRoutine callback = img->callback.contig;
    uint16 orientation;
    uint32 col, row;
    uint32 tw, th;
    u_char* buf;
    int32 fromskew;
    uint32 nrow;
    buf = (u_char*) _TIFFmalloc(TIFFTileSize(tif));
    if (buf == 0) {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), "No space for tile buffer");
	return (0);
    }
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
    orientation = img->orientation;
    for (row = 0; row < h; row += th) {
	nrow = (row + th > h ? h - row : th);
	for (col = 0; col < w; col += tw) {
	    if (TIFFReadTile(tif, buf, col, row, 0, 0) < 0 && img->stoponerr)
		break;
	    if (col + tw > w) {
		



		uint32 npix = w - col;
		fromskew = tw - npix;
		(*callback)(img, udata, col, row, npix, nrow, fromskew, buf);
	    } else {
		(*callback)(img, udata, col, row, tw, nrow, 0, buf);
	    }
	}
    }
    _TIFFfree(buf);
    return (1);
}

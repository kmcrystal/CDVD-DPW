static int
gtTileSeparate(TIFFImageIter* img, void *udata, uint32 w, uint32 h)
{
    TIFF* tif = img->tif;
    ImageIterTileSeparateRoutine callback = img->callback.separate;
    uint16 orientation;
    uint32 col, row;
    uint32 tw, th;
    u_char* buf;
    u_char* r;
    u_char* g;
    u_char* b;
    u_char* a;
    tsize_t tilesize;
    int32 fromskew;
    int alpha = img->alpha;
    uint32 nrow;
    tilesize = TIFFTileSize(tif);
    buf = (u_char*) _TIFFmalloc(4*tilesize);
    if (buf == 0) {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), "No space for tile buffer");
	return (0);
    }
    r = buf;
    g = r + tilesize;
    b = g + tilesize;
    a = b + tilesize;
    if (!alpha)
	memset(a, 0xff, tilesize);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
    orientation = img->orientation;
    for (row = 0; row < h; row += th) {
	nrow = (row + th > h ? h - row : th);
	for (col = 0; col < w; col += tw) {
	    if (TIFFReadTile(tif, r, col, row,0,0) < 0 && img->stoponerr)
		break;
	    if (TIFFReadTile(tif, g, col, row,0,1) < 0 && img->stoponerr)
		break;
	    if (TIFFReadTile(tif, b, col, row,0,2) < 0 && img->stoponerr)
		break;
	    if (alpha && TIFFReadTile(tif,a,col,row,0,3) < 0 && img->stoponerr)
		break;
	    if (col + tw > w) {
		



		uint32 npix = w - col;
		fromskew = tw - npix;
		(*callback)(img, udata, col, row, npix, nrow, fromskew, r, g, b, a);
	    } else {
		(*callback)(img, udata, col, row, tw, nrow, 0, r, g, b, a);
	    }
	}
    }
    _TIFFfree(buf);
    return (1);
}

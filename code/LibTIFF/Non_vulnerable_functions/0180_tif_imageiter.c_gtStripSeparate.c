static int
gtStripSeparate(TIFFImageIter* img, void *udata, uint32 w, uint32 h)
{
    TIFF* tif = img->tif;
    ImageIterTileSeparateRoutine callback = img->callback.separate;
    uint16 orientation;
    u_char *buf;
    u_char *r, *g, *b, *a;
    uint32 row, nrow;
    tsize_t scanline;
    uint32 rowsperstrip;
    uint32 imagewidth = img->width;
    tsize_t stripsize;
    int32 fromskew;
    int alpha = img->alpha;
    stripsize = TIFFStripSize(tif);
    r = buf = (u_char *)_TIFFmalloc(4*stripsize);
    if (buf == 0) {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), "No space for tile buffer");
	return (0);
    }
    g = r + stripsize;
    b = g + stripsize;
    a = b + stripsize;
    if (!alpha)
	memset(a, 0xff, stripsize);
    orientation = img->orientation;
    TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    scanline = TIFFScanlineSize(tif);
    fromskew = (w < imagewidth ? imagewidth - w : 0);
    for (row = 0; row < h; row += rowsperstrip) {
	nrow = (row + rowsperstrip > h ? h - row : rowsperstrip);
	if (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 0),
	    r, nrow*scanline) < 0 && img->stoponerr)
	    break;
	if (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 1),
	    g, nrow*scanline) < 0 && img->stoponerr)
	    break;
	if (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 2),
	    b, nrow*scanline) < 0 && img->stoponerr)
	    break;
	if (alpha &&
	    (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 3),
	    a, nrow*scanline) < 0 && img->stoponerr))
	    break;
	(*callback)(img, udata, 0, row, w, nrow, fromskew, r, g, b, a);
    }
    _TIFFfree(buf);
    return (1);
}

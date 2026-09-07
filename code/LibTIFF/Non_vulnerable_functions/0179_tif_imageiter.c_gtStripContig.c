static int
gtStripContig(TIFFImageIter* img, void *udata, uint32 w, uint32 h)
{
    TIFF* tif = img->tif;
    ImageIterTileContigRoutine callback = img->callback.contig;
    uint16 orientation;
    uint32 row, nrow;
    u_char* buf;
    uint32 rowsperstrip;
    uint32 imagewidth = img->width;
    tsize_t scanline;
    int32 fromskew;
    buf = (u_char*) _TIFFmalloc(TIFFStripSize(tif));
    if (buf == 0) {
	TIFFErrorExt(tif->tif_clientdata, TIFFFileName(tif), "No space for strip buffer");
	return (0);
    }
    orientation = img->orientation;
    TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    scanline = TIFFScanlineSize(tif);
    fromskew = (w < imagewidth ? imagewidth - w : 0);
    for (row = 0; row < h; row += rowsperstrip) {
	nrow = (row + rowsperstrip > h ? h - row : rowsperstrip);
	if (TIFFReadEncodedStrip(tif, TIFFComputeStrip(tif, row, 0),
	    buf, nrow*scanline) < 0 && img->stoponerr)
		break;
	(*callback)(img, udata, 0, row, w, nrow, fromskew, buf);
    }
    _TIFFfree(buf);
    return (1);
}

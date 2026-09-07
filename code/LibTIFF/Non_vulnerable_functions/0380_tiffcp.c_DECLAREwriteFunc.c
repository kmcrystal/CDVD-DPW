DECLAREwriteFunc(writeBufferToSeparateTiles)
{
	uint32 imagew = TIFFScanlineSize(out);
	tsize_t tilew  = TIFFTileRowSize(out);
	uint32 iimagew = TIFFRasterScanlineSize(out);
	int iskew = iimagew - tilew*spp;
	tsize_t tilesize = TIFFTileSize(out);
	tdata_t obuf;
	uint8* bufp = (uint8*) buf;
	uint32 tl, tw;
	uint32 row;
	uint16 bps = 0, bytes_per_sample;
	obuf = _TIFFmalloc(TIFFTileSize(out));
	if (obuf == NULL)
		return 0;
	_TIFFmemset(obuf, 0, tilesize);
	(void) TIFFGetField(out, TIFFTAG_TILELENGTH, &tl);
	(void) TIFFGetField(out, TIFFTAG_TILEWIDTH, &tw);
	(void) TIFFGetField(out, TIFFTAG_BITSPERSAMPLE, &bps);
        if( bps == 0 )
        {
            TIFFError(TIFFFileName(out), "Error, cannot read BitsPerSample");
            _TIFFfree(obuf);
            return 0;
        }
        if( (bps % 8) != 0 )
        {
            TIFFError(TIFFFileName(out), "Error, cannot handle BitsPerSample that is not a multiple of 8");
            _TIFFfree(obuf);
            return 0;
        }
	bytes_per_sample = bps/8;
	for (row = 0; row < imagelength; row += tl) {
		uint32 nrow = (row+tl > imagelength) ? imagelength-row : tl;
		uint32 colb = 0;
		uint32 col;
		for (col = 0; col < imagewidth; col += tw) {
			tsample_t s;
			for (s = 0; s < spp; s++) {
				



				if (colb + tilew > imagew) {
					uint32 width = (imagew - colb);
					int oskew = tilew - width;
					cpContigBufToSeparateBuf(obuf,
					    bufp + (colb*spp) + s,
					    nrow, width/bytes_per_sample,
					    oskew, (oskew*spp)+iskew, spp,
					    bytes_per_sample);
				} else
					cpContigBufToSeparateBuf(obuf,
					    bufp + (colb*spp) + s,
					    nrow, tilewidth,
					    0, iskew, spp,
					    bytes_per_sample);
				if (TIFFWriteTile(out, obuf, col, row, 0, s) < 0) {
					TIFFError(TIFFFileName(out),
					    "Error, can't write tile at %lu %lu "
					    "sample %lu",
					    (unsigned long) col,
					    (unsigned long) row,
					    (unsigned long) s);
					_TIFFfree(obuf);
					return 0;
				}
			}
			colb += tilew;
		}
		bufp += nrow * iimagew;
	}
	_TIFFfree(obuf);
	return 1;
}

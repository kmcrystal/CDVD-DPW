DECLAREreadFunc(readSeparateTilesIntoBuffer)
{
	int status = 1;
	uint32 imagew = TIFFRasterScanlineSize(in);
	uint32 tilew = TIFFTileRowSize(in);
	int iskew  = imagew - tilew*spp;
	tsize_t tilesize = TIFFTileSize(in);
	tdata_t tilebuf;
	uint8* bufp = (uint8*) buf;
	uint32 tw, tl;
	uint32 row;
	uint16 bps = 0, bytes_per_sample;
	tilebuf = _TIFFmalloc(tilesize);
	if (tilebuf == 0)
		return 0;
	_TIFFmemset(tilebuf, 0, tilesize);
	(void) TIFFGetField(in, TIFFTAG_TILEWIDTH, &tw);
	(void) TIFFGetField(in, TIFFTAG_TILELENGTH, &tl);
	(void) TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bps);
        if( bps == 0 )
        {
            TIFFError(TIFFFileName(in), "Error, cannot read BitsPerSample");
            status = 0;
            goto done;
        }
        if( (bps % 8) != 0 )
        {
            TIFFError(TIFFFileName(in), "Error, cannot handle BitsPerSample that is not a multiple of 8");
            status = 0;
            goto done;
        }
	bytes_per_sample = bps/8;
	for (row = 0; row < imagelength; row += tl) {
		uint32 nrow = (row+tl > imagelength) ? imagelength-row : tl;
		uint32 colb = 0;
		uint32 col;
		for (col = 0; col < imagewidth; col += tw) {
			tsample_t s;
			for (s = 0; s < spp; s++) {
				if (TIFFReadTile(in, tilebuf, col, row, 0, s) < 0
				    && !ignore) {
					TIFFError(TIFFFileName(in),
					    "Error, can't read tile at %lu %lu, "
					    "sample %lu",
					    (unsigned long) col,
					    (unsigned long) row,
					    (unsigned long) s);
					status = 0;
					goto done;
				}
				



				if (colb + tilew*spp > imagew) {
					uint32 width = imagew - colb;
					int oskew = tilew*spp - width;
					cpSeparateBufToContigBuf(
					    bufp+colb+s*bytes_per_sample,
					    tilebuf, nrow,
					    width/(spp*bytes_per_sample),
					    oskew + iskew,
					    oskew/spp, spp,
					    bytes_per_sample);
				} else
					cpSeparateBufToContigBuf(
					    bufp+colb+s*bytes_per_sample,
					    tilebuf, nrow, tw,
					    iskew, 0, spp,
					    bytes_per_sample);
			}
			colb += tilew*spp;
		}
		bufp += imagew * nrow;
	}
done:
	_TIFFfree(tilebuf);
	return status;
}

static void
ChopUpSingleUncompressedStrip(TIFF* tif)
{
	register TIFFDirectory *td = &tif->tif_dir;
	uint32 bytecount = td->td_stripbytecount[0];
	uint32 offset = td->td_stripoffset[0];
	tsize_t rowbytes = TIFFVTileSize(tif, 1), stripbytes;
	tstrip_t strip, nstrips, rowsperstrip;
	uint32* newcounts;
	uint32* newoffsets;
	



	if (rowbytes > 8192) {
		stripbytes = rowbytes;
		rowsperstrip = 1;
	} else {
		rowsperstrip = 8192 / rowbytes;
		stripbytes = rowbytes * rowsperstrip;
	}
	
	if (rowsperstrip >= td->td_rowsperstrip)
		return;
	nstrips = (tstrip_t) TIFFhowmany(bytecount, stripbytes);
	newcounts = (uint32*) CheckMalloc(tif, nstrips * sizeof (uint32),
				"for chopped \"StripByteCounts\" array");
	newoffsets = (uint32*) CheckMalloc(tif, nstrips * sizeof (uint32),
				"for chopped \"StripOffsets\" array");
	if (newcounts == NULL || newoffsets == NULL) {
	        



		if (newcounts != NULL)
			_TIFFfree(newcounts);
		if (newoffsets != NULL)
			_TIFFfree(newoffsets);
		return;
	}
	




	for (strip = 0; strip < nstrips; strip++) {
		if (stripbytes > bytecount)
			stripbytes = bytecount;
		newcounts[strip] = stripbytes;
		newoffsets[strip] = offset;
		offset += stripbytes;
		bytecount -= stripbytes;
	}
	


	td->td_stripsperimage = td->td_nstrips = nstrips;
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	_TIFFfree(td->td_stripbytecount);
	_TIFFfree(td->td_stripoffset);
	td->td_stripbytecount = newcounts;
	td->td_stripoffset = newoffsets;
}

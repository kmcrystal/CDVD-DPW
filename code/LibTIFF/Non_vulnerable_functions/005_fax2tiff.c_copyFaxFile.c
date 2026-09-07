int
copyFaxFile(TIFF* tifin, TIFF* tifout)
{
	uint32 row;
	uint32 linesize = TIFFhowmany8(xsize);
	uint16 badrun;
	int ok;
	tifin->tif_rawdatasize = (tmsize_t)TIFFGetFileSize(tifin);
	tifin->tif_rawdata = _TIFFmalloc(tifin->tif_rawdatasize);
	if (tifin->tif_rawdata == NULL) {
		TIFFError(tifin->tif_name, "Not enough memory");
		return (0);
	}
	if (!ReadOK(tifin, tifin->tif_rawdata, tifin->tif_rawdatasize)) {
		TIFFError(tifin->tif_name, "Read error at scanline 0");
		return (0);
	}
	tifin->tif_rawcp = tifin->tif_rawdata;
	tifin->tif_rawcc = tifin->tif_rawdatasize;
	(*tifin->tif_setupdecode)(tifin);
	(*tifin->tif_predecode)(tifin, (tsample_t) 0);
	tifin->tif_row = 0;
	badfaxlines = 0;
	badfaxrun = 0;
	_TIFFmemset(refbuf, 0, linesize);
	row = 0;
	badrun = 0;		
	while (tifin->tif_rawcc > 0) {
		ok = (*tifin->tif_decoderow)(tifin, (tdata_t) rowbuf, 
					     linesize, 0);
		if (!ok) {
			badfaxlines++;
			badrun++;
			
			_TIFFmemcpy(rowbuf, refbuf, linesize);
		} else {
			if (badrun > badfaxrun)
				badfaxrun = badrun;
			badrun = 0;
			_TIFFmemcpy(refbuf, rowbuf, linesize);
		}
		tifin->tif_row++;
		if (TIFFWriteScanline(tifout, rowbuf, row, 0) < 0) {
			fprintf(stderr, "%s: Write error at row %ld.\n",
			    tifout->tif_name, (long) row);
			break;
		}
		row++;
		if (stretch) {
			if (TIFFWriteScanline(tifout, rowbuf, row, 0) < 0) {
				fprintf(stderr, "%s: Write error at row %ld.\n",
				    tifout->tif_name, (long) row);
				break;
			}
			row++;
		}
	}
	if (badrun > badfaxrun)
		badfaxrun = badrun;
	_TIFFfree(tifin->tif_rawdata);
	return (row);
}

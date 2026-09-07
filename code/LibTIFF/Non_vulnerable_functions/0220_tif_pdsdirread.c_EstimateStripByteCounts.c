static void
EstimateStripByteCounts(TIFF* tif, TIFFDirEntry* dir, uint16 dircount)
{
	register TIFFDirEntry *dp;
	register TIFFDirectory *td = &tif->tif_dir;
	uint16 i;
	if (td->td_stripbytecount)
		_TIFFfree(td->td_stripbytecount);
	td->td_stripbytecount = (uint32*)
	    CheckMalloc(tif, td->td_nstrips * sizeof (uint32),
		"for \"StripByteCounts\" array");
	if (td->td_compression != COMPRESSION_NONE) {
		uint32 space = (uint32)(sizeof (TIFFHeader)
		    + sizeof (uint16)
		    + (dircount * sizeof (TIFFDirEntry))
		    + sizeof (uint32));
		toff_t filesize = TIFFGetFileSize(tif);
		uint16 n;
		
		for (dp = dir, n = dircount; n > 0; n--, dp++) {
			uint32 cc = dp->tdir_count*TIFFDataWidth(dp->tdir_type);
			if (cc > sizeof (uint32))
				space += cc;
		}
		space = (filesize - space) / td->td_samplesperpixel;
		for (i = 0; i < td->td_nstrips; i++)
			td->td_stripbytecount[i] = space;
		





 
		i--;
		if (td->td_stripoffset[i] + td->td_stripbytecount[i] > filesize)
			td->td_stripbytecount[i] =
			    filesize - td->td_stripoffset[i];
	} else {
		uint32 rowbytes = TIFFScanlineSize(tif);
		uint32 rowsperstrip = td->td_imagelength / td->td_nstrips;
		for (i = 0; i < td->td_nstrips; i++)
			td->td_stripbytecount[i] = rowbytes*rowsperstrip;
	}
	TIFFSetFieldBit(tif, FIELD_STRIPBYTECOUNTS);
	if (!TIFFFieldSet(tif, FIELD_ROWSPERSTRIP))
		td->td_rowsperstrip = td->td_imagelength;
}

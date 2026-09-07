void
TIFFFreeDirectory(TIFF* tif)
{
	TIFFDirectory *td = &tif->tif_dir;
	int            i;
	_TIFFmemset(td->td_fieldsset, 0, FIELD_SETLONGS);
	CleanupField(td_colormap[0]);
	CleanupField(td_colormap[1]);
	CleanupField(td_colormap[2]);
	CleanupField(td_sampleinfo);
	CleanupField(td_subifd);
	CleanupField(td_inknames);
	CleanupField(td_transferfunction[0]);
	CleanupField(td_transferfunction[1]);
	CleanupField(td_transferfunction[2]);
	CleanupField(td_stripoffset);
	CleanupField(td_stripbytecount);
	TIFFClrFieldBit(tif, FIELD_YCBCRSUBSAMPLING);
	TIFFClrFieldBit(tif, FIELD_YCBCRPOSITIONING);
	
	for( i = 0; i < td->td_customValueCount; i++ ) {
		if (td->td_customValues[i].value)
			_TIFFfree(td->td_customValues[i].value);
	}
	td->td_customValueCount = 0;
	CleanupField(td_customValues);
}

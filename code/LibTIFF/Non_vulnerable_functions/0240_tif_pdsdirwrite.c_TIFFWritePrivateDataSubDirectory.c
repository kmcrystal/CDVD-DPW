toff_t
TIFFWritePrivateDataSubDirectory(TIFF* tif,
				 uint32 pdir_fieldsset[], int pdir_fields_last,
				 TIFFFieldInfo *field_info,
				 int (*getFieldFn)(TIFF *tif, ttag_t tag, ...))
{
	uint16 dircount;
	uint32 diroff, nextdiroff;
	ttag_t tag;
	uint32 nfields;
	tsize_t dirsize;
	char* data;
	TIFFDirEntry* dir;
	u_long b, *fields, fields_size;
	toff_t directory_offset;
	TIFFFieldInfo* fip;
	



	
	TIFFFlushData(tif);
	




	nfields = 0;
	for (b = 0; b <= pdir_fields_last; b++)
		if (FieldSet(pdir_fieldsset, b))
			

			nfields += 1;
	dirsize = nfields * sizeof (TIFFDirEntry);
	data = (char*) _TIFFmalloc(dirsize);
	if (data == NULL) {
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
		    "Cannot write private subdirectory, out of space");
		return (0);
	}
	




	if (tif->tif_dataoff == 0)
	    tif->tif_dataoff =(TIFFSeekFile(tif, (toff_t) 0, SEEK_END)+1) &~ 1;
	diroff = tif->tif_dataoff;
	tif->tif_dataoff = (toff_t)(
	    diroff + sizeof (uint16) + dirsize + sizeof (toff_t));
	if (tif->tif_dataoff & 1)
		tif->tif_dataoff++;
	(void) TIFFSeekFile(tif, tif->tif_dataoff, SEEK_SET);
	
	dir = (TIFFDirEntry*) data;
	



	















	fields_size = pdir_fields_last / (8*sizeof(uint32)) + 1;
	fields = _TIFFmalloc(fields_size*sizeof(uint32));
	_TIFFmemcpy(fields, pdir_fieldsset, fields_size * sizeof(uint32));
	
	



	for (fip = field_info; fip->field_tag; fip++) {
		
		if (
		    !FieldSet(fields, fip->field_bit))
			continue;
		if (!TIFFWriteNormalSubTag(tif, dir, fip, getFieldFn))
			goto bad;
		dir++;
		ResetFieldBit(fields, fip->field_bit);
	}
	


	directory_offset = tif->tif_dataoff;
	


	dircount = (uint16) nfields;
	
	nextdiroff = 0;
	if (tif->tif_flags & TIFF_SWAB) {
		









		for (dir = (TIFFDirEntry*) data; dircount; dir++, dircount--) {
			TIFFSwabArrayOfShort(&dir->tdir_tag, 2);
			TIFFSwabArrayOfLong(&dir->tdir_count, 2);
		}
		dircount = (uint16) nfields;
		TIFFSwabShort(&dircount);
		TIFFSwabLong(&nextdiroff);
	}
	(void) TIFFSeekFile(tif, tif->tif_dataoff, SEEK_SET);
	if (!WriteOK(tif, &dircount, sizeof (dircount))) {
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Error writing private subdirectory count");
		goto bad;
	}
	if (!WriteOK(tif, data, dirsize)) {
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Error writing private subdirectory contents");
		goto bad;
	}
	if (!WriteOK(tif, &nextdiroff, sizeof (nextdiroff))) {
		TIFFErrorExt(tif->tif_clientdata, tif->tif_name, "Error writing private subdirectory link");
		goto bad;
	}
	tif->tif_dataoff += sizeof(dircount) + dirsize + sizeof(nextdiroff);
	_TIFFfree(data);
	_TIFFfree(fields);
	tif->tif_flags &= ~TIFF_DIRTYDIRECT;
	

	(*tif->tif_cleanup)(tif);
	



	TIFFDefaultDirectory(tif);
	tif->tif_curoff = 0;
	tif->tif_row = (uint32) -1;
	tif->tif_curstrip = (tstrip_t) -1;
	return (directory_offset);
bad:
	_TIFFfree(data);
	_TIFFfree(fields);
	return (0);
}

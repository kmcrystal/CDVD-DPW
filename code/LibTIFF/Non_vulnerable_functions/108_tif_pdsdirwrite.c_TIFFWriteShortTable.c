static int
TIFFWriteShortTable(TIFF* tif,
    ttag_t tag, TIFFDirEntry* dir, uint32 n, uint16** table)
{
	uint32 i, off;
	dir->tdir_tag = tag;
	dir->tdir_type = (short) TIFF_SHORT;
	
	dir->tdir_count = (uint32) (1L<<tif->tif_dir.td_bitspersample);
	off = tif->tif_dataoff;
	for (i = 0; i < n; i++)
		if (!TIFFWriteData(tif, dir, (char *)table[i]))
			return (0);
	dir->tdir_count *= n;
	dir->tdir_offset = off;
	return (1);
}

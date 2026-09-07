static void
TIFFSetupShortLong(TIFF* tif, ttag_t tag, TIFFDirEntry* dir, uint32 v)
{
	dir->tdir_tag = tag;
	dir->tdir_count = 1;
	if (v > 0xffffL) {
		dir->tdir_type = (short) TIFF_LONG;
		dir->tdir_offset = v;
	} else {
		dir->tdir_type = (short) TIFF_SHORT;
		dir->tdir_offset = TIFFInsertData(tif, (int) TIFF_SHORT, v);
	}
}

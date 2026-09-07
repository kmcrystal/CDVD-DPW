static int
TIFFWriteLongArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, uint32* v)
{
	dir->tdir_tag = tag;
	dir->tdir_type = (short) type;
	dir->tdir_count = n;
	if (n == 1) {
		dir->tdir_offset = v[0];
		return (1);
	} else
		return (TIFFWriteData(tif, dir, (char*) v));
}

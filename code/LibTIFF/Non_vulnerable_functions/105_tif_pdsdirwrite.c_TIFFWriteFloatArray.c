static int
TIFFWriteFloatArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, float* v)
{
	dir->tdir_tag = tag;
	dir->tdir_type = (short) type;
	dir->tdir_count = n;
	TIFFCvtNativeToIEEEFloat(tif, n, v);
	if (n == 1) {
		dir->tdir_offset = *(uint32*) &v[0];
		return (1);
	} else
		return (TIFFWriteData(tif, dir, (char*) v));
}

static int
TIFFWriteDoubleArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, double* v)
{
	dir->tdir_tag = tag;
	dir->tdir_type = (short) type;
	dir->tdir_count = n;
	TIFFCvtNativeToIEEEDouble(tif, n, v);
	return (TIFFWriteData(tif, dir, (char*) v));
}

static int
TIFFFetchFloatArray(TIFF* tif, TIFFDirEntry* dir, float* v)
{
	if (dir->tdir_count == 1) {
		v[0] = *(float*) &dir->tdir_offset;
		TIFFCvtIEEEFloatToNative(tif, dir->tdir_count, v);
		return (1);
	} else	if (TIFFFetchData(tif, dir, (char*) v)) {
		TIFFCvtIEEEFloatToNative(tif, dir->tdir_count, v);
		return (1);
	} else
		return (0);
}

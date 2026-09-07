static int
TIFFWriteRational(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, float v)
{
	return (TIFFWriteRationalArray(tif, type, tag, dir, 1, &v));
}
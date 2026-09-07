static float
TIFFFetchFloat(TIFF* tif, TIFFDirEntry* dir)
{
	


#if (0)
	float v = (float)
	    TIFFExtractData(tif, dir->tdir_type, dir->tdir_offset);
	TIFFCvtIEEEFloatToNative(tif, 1, &v);
#else
	float v;
	




	uint32 l = (uint32)
	    TIFFExtractData(tif, dir->tdir_type, dir->tdir_offset);
	v = * (float *) &l;
	TIFFCvtIEEEFloatToNative(tif, 1, &v);
#endif
	return (v);
}

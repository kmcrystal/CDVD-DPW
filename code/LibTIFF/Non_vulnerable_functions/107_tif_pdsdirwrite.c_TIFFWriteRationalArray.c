static int
TIFFWriteRationalArray(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir, uint32 n, float* v)
{
	uint32 i;
	uint32* t;
	int status;
	dir->tdir_tag = tag;
	dir->tdir_type = (short) type;
	dir->tdir_count = n;
	t = (uint32*) _TIFFmalloc(2*n * sizeof (uint32));
	for (i = 0; i < n; i++) {
		float fv = v[i];
		int sign = 1;
		uint32 den;
		if (fv < 0) {
			if (type == TIFF_RATIONAL) {
				TIFFWarning(tif->tif_name,
	"\"%s\": Information lost writing value (%g) as (unsigned) RATIONAL",
				_TIFFFieldWithTag(tif,tag)->field_name, v);
				fv = 0;
			} else
				fv = -fv, sign = -1;
		}
		den = 1L;
		if (fv > 0) {
			while (fv < 1L<<(31-3) && den < 1L<<(31-3))
				fv *= 1<<3, den *= 1L<<3;
		}
		t[2*i+0] = sign * (fv + 0.5);
		t[2*i+1] = den;
	}
	status = TIFFWriteData(tif, dir, (char *)t);
	_TIFFfree((char*) t);
	return (status);
}

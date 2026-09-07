static int
TIFFFetchPerSampleAnys(TIFF* tif, TIFFDirEntry* dir, double* pl)
{
	int samples = (int) tif->tif_dir.td_samplesperpixel;
	int status = 0;
	if (CheckDirCount(tif, dir, (uint32) samples)) {
		double buf[10];
		double* v = buf;
		if (samples > NITEMS(buf))
			v = (double*) _TIFFmalloc(samples * sizeof (double));
		if (TIFFFetchAnyArray(tif, dir, v)) {
			int i;
			for (i = 1; i < samples; i++)
				if (v[i] != v[0]) {
					TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
		"Cannot handle different per-sample values for field \"%s\"",
			   _TIFFFieldWithTag(tif, dir->tdir_tag)->field_name);
					goto bad;
				}
			*pl = v[0];
			status = 1;
		}
	bad:
		if (v != buf)
			_TIFFfree(v);
	}
	return (status);
}

static int
TIFFFetchPerSampleShorts(TIFF* tif, TIFFDirEntry* dir, int* pl)
{
	int samples = tif->tif_dir.td_samplesperpixel;
	int status = 0;
	if (CheckDirCount(tif, dir, (uint32) samples)) {
		uint16 buf[10];
		uint16* v = buf;
		if (samples > NITEMS(buf))
			v = (uint16*) _TIFFmalloc(samples * sizeof (uint16));
		if (TIFFFetchShortArray(tif, dir, v)) {
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
			_TIFFfree((char*) v);
	}
	return (status);
}

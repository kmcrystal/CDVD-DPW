static int
TIFFWritePerSampleAnys(TIFF* tif,
    TIFFDataType type, ttag_t tag, TIFFDirEntry* dir)
{
	double buf[10], v;
	double* w = buf;
	int i, status;
	int samples = (int) tif->tif_dir.td_samplesperpixel;
	if (samples > NITEMS(buf))
		w = (double*) _TIFFmalloc(samples * sizeof (double));
	TIFFGetField(tif, tag, &v);
	for (i = 0; i < samples; i++)
		w[i] = v;
	status = TIFFWriteAnyArray(tif, type, tag, dir, samples, w);
	if (w != buf)
		_TIFFfree(w);
	return (status);
}

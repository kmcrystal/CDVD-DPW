static int
TIFFWritePerSampleShorts(TIFF* tif, ttag_t tag, TIFFDirEntry* dir)
{
	uint16 buf[10], v;
	uint16* w = buf;
	int i, status, samples = tif->tif_dir.td_samplesperpixel;
	if (samples > NITEMS(buf))
		w = (uint16*) _TIFFmalloc(samples * sizeof (uint16));
	TIFFGetField(tif, tag, &v);
	for (i = 0; i < samples; i++)
		w[i] = v;
	status = TIFFWriteShortArray(tif, TIFF_SHORT, tag, dir, samples, w);
	if (w != buf)
		_TIFFfree((char*) w);
	return (status);
}
